
#include "sentinel-datasource-extension/datasource/abstract_readable_data_source.h"

#include "sentinel-core/test/mock/common/mock.h"
#include "sentinel-core/test/mock/property/mock.h"
#include "sentinel-datasource-extension/test/mock/datasource/mock.h"

namespace Sentinel {
namespace DataSource {

using testing::_;
using testing::An;
using testing::InSequence;
using testing::Matcher;

TEST(AbstractReadableDataSourceTest, Basic) {
  auto *convert = new MockConverter<int, FakeRule>();

  ConverterSharedPtr<int, FakeRule> convert_shared_ptr;
  convert_shared_ptr.reset(convert);

  MockAbstractReabableDataSource<int, FakeRule> fake_data_source(
      convert_shared_ptr);
  FakeRule rule;
  rule.set_fake_rule_data("test");
  auto pl = std::make_unique<Property::MockPropertyListener<FakeRule>>();
  auto pl_point = pl.get();

  EXPECT_CALL(*pl_point, ConfigUpdate(Matcher<const FakeRule &>(rule), false))
      .Times(1);

  fake_data_source.GetProperty()->AddListener(std::move(pl));
  fake_data_source.GetProperty()->UpdateValue(rule);

  EXPECT_CALL(*convert, Convert(_)).Times(1);
  EXPECT_CALL(fake_data_source, ReadSource()).Times(1);

  fake_data_source.LoadConfig();
};

}  // namespace DataSource
}  // namespace Sentinel