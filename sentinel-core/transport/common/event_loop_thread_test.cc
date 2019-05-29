#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define private public

#include "sentinel-core/transport/common/event_loop_thread.h"

namespace Sentinel {
    namespace Transport {

TEST(CommandRequestTest, TestStart) {
   EventLoopThread loop;
   auto ret = loop.Start();
   EXPECT_EQ(ret, true);
    EXPECT_EQ(loop.stoped_, false);
   EXPECT_NE(loop.thd_->get_id(), std::this_thread::get_id());
}


TEST(CommandRequestTest, TestStop) {
   EventLoopThread loop;
   auto ret = loop.Start();
   EXPECT_EQ(ret, true);

   loop.Stop();
    EXPECT_EQ(loop.stoped_, true);
   EXPECT_EQ(loop.thd_->joinable(), false);
}


TEST(CommandRequestTest, TestRunTask) {
   EventLoopThread loop;
   auto ret = loop.Start();
   EXPECT_EQ(ret, true);

  std::promise<std::thread::id> promise;
  auto future = promise.get_future();

  auto task = [&promise]() { promise.set_value(std::this_thread::get_id()); };

  loop.RunTask(task);

  auto run_task_thread_id = future.get();
  EXPECT_EQ(run_task_thread_id, loop.thd_->get_id());

   loop.Stop();
    EXPECT_EQ(loop.stoped_, true);
   EXPECT_EQ(loop.thd_->joinable(), false);
}

}  // namespace Transport
}  // namespace Sentinel