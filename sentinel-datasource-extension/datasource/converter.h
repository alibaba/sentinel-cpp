#pragma once

#include <memory>

#include "absl/types/optional.h"

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class Converter {
 public:
  virtual ~Converter() = default;
  virtual absl::optional<T> Convert(const S& source) = 0;
};

template <typename S, typename T>
using ConverterSharedPtr = std::shared_ptr<Converter<S, T>>;

}  // namespace DataSource
}  // namespace Sentinel
