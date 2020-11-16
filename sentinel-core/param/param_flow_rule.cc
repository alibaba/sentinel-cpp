#include "sentinel-core/param/param_flow_rule.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace Param {

void ParamFlowRule::FillExceptionFlowItems() const {
  if (param_flow_item_list_.size() == 0) {
    return;
  }
  for (auto& item : param_flow_item_list_) {
    hot_items_->insert(std::make_pair<>(item.param_value(), item.threshold()));
  }
}

bool ParamFlowRule::operator==(const ParamFlowRule& rule) const {
  if (!(resource_ == rule.resource() && param_idx_ == rule.param_idx() &&
        metric_type_ == rule.metric_type() && threshold_ == rule.threshold() &&
        interval_in_ms_ == rule.interval_in_ms() &&
        sample_count_ == rule.sample_count() &&
        cache_size_ == rule.cache_size() &&
        cluster_mode_ == rule.cluster_mode() &&
        param_flow_item_list_.size() == rule.param_flow_item_list_.size())) {
    return false;
  }
  for (int i = 0; i < param_flow_item_list_.size(); i++) {
    if (!(rule.param_flow_item_list_[i] == param_flow_item_list_[i])) {
      return false;
    }
  }
  return true;
}

std::string ParamFlowRule::ToString() const {
  return absl::StrFormat(
      "ParamFlowRule{resource=%s, index=%d, metric_type=%d, "
      "threshold=%.2f, interval_in_ms=%d, sample_count=%d, "
      "cache_size=%d, cluster_mode=%d, exception_list=%s}",
      resource_, param_idx_, static_cast<int32_t>(metric_type_), threshold_,
      interval_in_ms_, sample_count_, cache_size_, cluster_mode_,
      param_flow_item_list_.ToString());
}

}  // namespace Param
}  // namespace Sentinel
