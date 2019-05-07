#pragma once

#include <memory>

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class Converter {
 public:
  virtual ~Converter() = default;
  virtual T Convert(S source) = 0;
};

template <typename S, typename T>
using ConverterSharedPtr = std::shared_ptr<Converter<S, T>>;

}  // namespace DataSource
}  // namespace Sentinel
