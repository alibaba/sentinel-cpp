#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <vector>

#include <event2/event.h>

#include "absl/synchronization/mutex.h"

namespace Sentinel {
namespace Transport {

class EventLoopThread {
  using Functor = std::function<void()>;

 public:
  EventLoopThread();
  ~EventLoopThread() = default;

  bool Start();
  void Stop();

  struct event_base *GetEventBase();

  void RunTask(Functor &&func);

  bool IsInLoopThread() const;

 private:
  bool InitEventBase();
  void ClearEventBase();

  void Dispatch();
  void Work(std::promise<bool> &&promise);
  void Wakeup();
  void DoPendingTasks();

  static void OnWakeupFdCallback(evutil_socket_t fd, short events,
                                 void *userdata);

 private:
  // TODO(tianqian.zyf): Use RAII wrapper event_base
  struct event_base *base_ = nullptr;

  std::unique_ptr<std::thread> thd_;
  std::atomic<bool> stoped_{true};
  evutil_socket_t wakeup_fd_[2];  // 0:read 1:write

  absl::Mutex task_mutex_;
  std::vector<Functor> pending_tasks_ GUARDED_BY(task_mutex_);
};

}  // namespace Transport
}  // namespace Sentinel