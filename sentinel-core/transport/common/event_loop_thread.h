#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <vector>

#include <event2/event.h>

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

  void RunTask(Functor func);

 private:
  bool InitEventBase();
  void ClearEventBase();

  void Dispatch();
  void Work(std::promise<bool> &promise);
  void Wakeup();
  void DoPendingTasks();

  static void OnWakeupFdCallback(evutil_socket_t fd, short events,
                                 void *userdata);

 private:
  struct event_base *base_ = nullptr;

  std::unique_ptr<std::thread> thd_;
  std::atomic<bool> stoped_;
  evutil_socket_t wakeup_fd_[2];  // 0:read 1:write

  std::mutex task_mutex_;
  std::vector<Functor> pending_tasks_;
};

}  // namespace Transport
}  // namespace Sentinel