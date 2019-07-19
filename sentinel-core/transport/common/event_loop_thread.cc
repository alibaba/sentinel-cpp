#include "sentinel-core/transport/common/event_loop_thread.h"

#ifndef WIN32
#include <unistd.h>
#endif  // !WIN32

namespace Sentinel {
namespace Transport {

EventLoopThread::EventLoopThread() = default;

bool EventLoopThread::Start() {
  std::promise<bool> start_promise;
  auto start_future = start_promise.get_future();

  thd_.reset(new std::thread(
      [start_promise = std::move(start_promise), this]() mutable {
        this->Work(std::move(start_promise));
      }));

  return start_future.get();
}

void EventLoopThread::Stop() {
  bool expected = false;
  if (!stoped_.compare_exchange_strong(expected, true)) {
    return;
  }

  Wakeup();

  thd_->join();
}

void EventLoopThread::Work(std::promise<bool>&& promise) {
  auto ret = InitEventBase();
  if (!ret) {
    promise.set_value(false);
    return;
  }

  promise.set_value(true);

  Dispatch();

  ClearEventBase();
}

bool EventLoopThread::InitEventBase() {
  base_ = event_base_new();
  if (!base_) {
    // log error
    return false;
  }

  evutil_socket_t pair[2] = {-1, -1};
  if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) < 0) {
    // log error
    return false;
  }

  if (evutil_make_socket_nonblocking(pair[0]) < 0) {
    // log error
    return false;
  }
  if (evutil_make_socket_closeonexec(pair[0]) < 0) {
    // log error
    return false;
  }
  if (evutil_make_socket_nonblocking(pair[1]) < 0) {
    // log error
    return false;
  }
  if (evutil_make_socket_closeonexec(pair[1]) < 0) {
    // log error
    return false;
  }

  wakeup_fd_[0] = pair[0];
  wakeup_fd_[1] = pair[1];

  auto wake_read_event = event_new(base_, wakeup_fd_[0], EV_READ | EV_PERSIST,
                                   &EventLoopThread::OnWakeupFdCallback, this);

  event_add(wake_read_event, nullptr);
  return true;
}

void EventLoopThread::ClearEventBase() {
  if (base_) {
    event_base_free(base_);
    base_ = nullptr;
  }
}

void EventLoopThread::Dispatch() {
  stoped_.store(false);

  while (!stoped_.load()) {
    event_base_loop(base_, EVLOOP_ONCE);
    DoPendingTasks();
  }
}

void EventLoopThread::RunTask(Functor&& func) {
  if (IsInLoopThread()) {
    func();
    return;
  }

  {
    absl::WriterMutexLock lck(&task_mutex_);
    pending_tasks_.emplace_back(std::move(func));
  }

  Wakeup();
}

void EventLoopThread::Wakeup() {
  uint64_t chr = 1;
#ifndef WIN32
  ::write(wakeup_fd_[1], &chr, sizeof chr);
#else
  send(wakeup_fd_[1], (const char*)&chr, sizeof chr, 0);
#endif  // !WIN32
}

void EventLoopThread::OnWakeupFdCallback(evutil_socket_t fd, short, void*) {
  uint64_t chr;
#ifndef WIN32
  ::read(fd, &chr, sizeof chr);
#else
  while (recv(fd, (char*)&chr, sizeof chr, 0) > 0)
    ;
#endif  // !WIN32
}

struct event_base* EventLoopThread::GetEventBase() {
  return base_;
}

void EventLoopThread::DoPendingTasks() {
  std::vector<Functor> functors;

  {
    absl::WriterMutexLock lck(&task_mutex_);
    functors.swap(pending_tasks_);
  }

  for (const Functor& functor : functors) {
    functor();
  }
}

bool EventLoopThread::IsInLoopThread() const {
  return thd_->get_id() == std::this_thread::get_id();
}

}  // namespace Transport
}  // namespace Sentinel
