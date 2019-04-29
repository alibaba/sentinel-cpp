#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/property/dynamic_sentinel_property.h"
#include "sentinel-core/test/mock/common/mock.h"
#include "sentinel-core/test/mock/property/mock.h"

using testing::_;
using testing::InSequence;
using testing::Matcher;
using testing::Return;
using testing::ReturnRef;

namespace Sentinel {
namespace Property {

TEST(DynamicSentinelPropertyTest, Basic) {
  FakeRule rule;
  rule.set_fake_rule_data("test");
  DynamicSentinelProperty<FakeRule> ds;
  auto pl = std::make_unique<MockPropertyListener<FakeRule>>();
  auto pl_point = pl.get();

  EXPECT_CALL(*pl_point, ConfigUpdate(Matcher<const FakeRule&>(rule), false))
      .Times(1);

  ds.AddListener(std::move(pl));
  ds.UpdateValue(rule);
}

}  // namespace Property
}  // namespace Sentinel
