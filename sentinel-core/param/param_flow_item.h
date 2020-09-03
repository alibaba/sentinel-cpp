#pragma once

#include <string>
#include "absl/types/any.h"

namespace Sentinel {
namespace Param {

class ParamFlowItem {
 public:
  ParamFlowItem() = default;
  ParamFlowItem(absl::any param_value, int threshold, std::string param_type)
      : param_value_(param_value),
        threshold_(threshold),
        param_type_(param_type) {}

  std::string param_type() const { return param_type_; }
  absl::any param_value() const { return param_value_; }
  int threshold() const { return threshold_; }

  void set_param_type(std::string param_type) { param_type_ = param_type; }
  void set_param_value(absl::any param_value) { param_value_ = param_value; }
  void set_threshold(int threshold) { threshold_ = threshold; }

 private:
  std::string param_type_;
  absl::any param_value_;
  int threshold_ = -1;
};

class ParamFlowItemHash {
  std::size_t operator()(const ParamFlowItem& item) const noexcept {
    std::size_t result = std::hash<std::string>{}(item.param_type());
    // result = 31 * result + static_cast<int>(item.param_value_()); // TODO:
    // ???
    result = 31 * result + item.threshold();
    return result;
  }
};

}  // namespace Param
}  // namespace Sentinel