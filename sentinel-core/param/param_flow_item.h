#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include "absl/types/any.h"
#include "sentinel-core/param/statistic/any_cmp.h"

namespace Sentinel {
namespace Param {

class ParamFlowItem {
 public:
  ParamFlowItem() : threshold_(-1) {}
  ParamFlowItem(absl::any param_value, std::string param_type,
                int threshold = -1)
      : param_value_(param_value),
        threshold_(threshold),
        param_type_(param_type) {}

  std::string param_type() const { return param_type_; }
  absl::any param_value() const { return param_value_; }
  int threshold() const { return threshold_; }

  void set_param_type(std::string param_type) { param_type_ = param_type; }
  void set_param_value(absl::any param_value) { param_value_ = param_value; }
  void set_threshold(int threshold) { threshold_ = threshold; }
  bool operator==(const ParamFlowItem& item) const noexcept;
  int HashCode() const;
  std::string ToString() const;

 private:
  std::string param_type_;
  absl::any param_value_;
  int threshold_ = -1;
};

class ParamFlowItemList : public std::vector<ParamFlowItem> {
 public:
  ParamFlowItemList() = default;
  ParamFlowItemList(std::initializer_list<ParamFlowItem> args);
  bool operator==(const ParamFlowItemList& list) const noexcept;
  int HashCode() const noexcept;
  std::string ToString() const;
};

}  // namespace Param
}  // namespace Sentinel