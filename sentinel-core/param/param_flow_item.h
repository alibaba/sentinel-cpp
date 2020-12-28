#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include "absl/types/any.h"
#include "sentinel-core/param/param_flow_rule_constants.h"
#include "sentinel-core/param/statistic/any_cmp.h"

namespace Sentinel {
namespace Param {

class ParamFlowItem {
 public:
  ParamFlowItem() : threshold_(-1) {}
  ParamFlowItem(absl::any param_value, ParamItemType param_type,
                double threshold = -1)
      : param_value_(param_value),
        threshold_(threshold),
        param_type_(param_type) {}

  ParamItemType param_type() const { return param_type_; }
  absl::any param_value() const { return param_value_; }
  double threshold() const { return threshold_; }

  void set_param_type(ParamItemType param_type) { param_type_ = param_type; }
  void set_param_value(absl::any param_value) { param_value_ = param_value; }
  void set_threshold(double threshold) { threshold_ = threshold; }
  bool operator==(const ParamFlowItem& item) const noexcept;
  std::string ToString() const;

 private:
  ParamItemType param_type_;
  absl::any param_value_;
  double threshold_ = -1;
};

class ParamFlowItemList : public std::vector<ParamFlowItem> {
 public:
  ParamFlowItemList() = default;
  ParamFlowItemList(std::initializer_list<ParamFlowItem> args);
  bool operator==(const ParamFlowItemList& list) const noexcept;
  std::string ToString() const;
};

}  // namespace Param
}  // namespace Sentinel