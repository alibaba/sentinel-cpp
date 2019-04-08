#pragma once

namespace Sentinel {
namespace DataSource {

template <typename S, typename T>
class Converter {
 public:
  virtual ~Converter() = default;
  virtual T Convert(S source) = 0;
};

}
}
