#pragma once

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-datasource-extension/datasource/abstract_readable_data_source.h"
#include "sentinel-datasource-extension/datasource/converter.h"

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class MockConverter : public Converter<S, T> {
 public:
  virtual ~MockConverter() = default;
  MOCK_METHOD1_T(Convert, T(S));
};

template <typename S, typename T>
class MockAbstractReabableDataSource : public AbstractReadableDataSource<S, T> {
 public:
  using AbstractReadableDataSource<S, T>::AbstractReadableDataSource;

  virtual ~MockAbstractReabableDataSource() = default;
  MOCK_METHOD0_T(ReadSource, S());
};

}  // namespace DataSource
}  // namespace Sentinel
