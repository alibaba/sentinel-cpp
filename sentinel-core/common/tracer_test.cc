#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/common/tracer.h"
#include "sentinel-core/test/mock/statistic/node/mock.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {

TEST(TracerTest, TraceExceptionTest) {
  {
    auto node = std::make_shared<Stat::MockNode>();
    int n = 10;

    EXPECT_CALL(*node.get(), AddExceptionRequest(n)).Times(1);

    EntrySharedPtr entry = std::make_shared<Entry>(
        std::make_shared<StringResourceWrapper>("abc", EntryType::IN));
    entry->SetCurNode(node);

    Tracer::Trace(entry, "some_exception", n);
  }
}

}  // namespace Sentinel