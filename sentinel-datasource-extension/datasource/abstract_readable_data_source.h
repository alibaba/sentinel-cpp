#pragma once

#include "sentinel-core/property/dynamic_sentinel_property.h"
#include "sentinel-datasource-extension/datasource/converter.h"
#include "sentinel-datasource-extension/datasource/readable_data_source.h"

#include <memory>
#include <string>

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class AbstractReadableDataSource : public ReadableDataSource<S, T> {
 public:
  explicit AbstractReadableDataSource(ConverterSharedPtr<S, T> converter)
      : parser_(converter),
        property_(std::make_shared<Property::DynamicSentinelProperty<T>>()) {}
  virtual ~AbstractReadableDataSource() = default;

  T LoadConfig() override { return parser_->Convert(this->ReadSource()); }
  Property::SentinelPropertySharedPtr<T> GetProperty() override {
    return property_;
  }

 private:
  ConverterSharedPtr<S, T> parser_;
  Property::SentinelPropertySharedPtr<T> property_;
};

}  // namespace DataSource
}  // namespace Sentinel
