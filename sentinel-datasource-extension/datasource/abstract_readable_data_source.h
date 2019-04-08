#pragma once

#include "sentinel-core/property/dynamic_sentinel_property.h"
#include "sentinel-datasource-extension/datasource/converter.h"

#include <memory>
#include <string>

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class AbstractReadableDataSource : public ReadableDataSource<S, T> {
 public:
  explicit AbstractReadableDataSource(Converter& converter);
  virtual ~AbstractReadableDataSource() = default;

  T LoadConfig() override;
  std::shared_ptr<Property::SentinelProperty> GetProperty() override;
 private:
  std::shared_ptr<Converter> parser_;
  std::shared_ptr<SentinelProperty> property_;

  T LoadConfig(S conf);
};

}
}
