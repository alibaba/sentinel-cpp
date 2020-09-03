#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "absl/types/any.h"
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/rule.h"
#include "sentinel-core/param/param_flow_item.h"
#include "sentinel-core/param/param_flow_rule_constants.h"

namespace Sentinel {
namespace Param {

// Deep copy safe
class ParamFlowRule : public Rule {
 public:
  class ParamLeapArrayKey {
   public:
    int interval_in_ms_ = 1000;
    int param_idx_ = -1;
  };
  using ParamLeapArrayKeySharedPtr = std::shared_ptr<ParamLeapArrayKey>;

  class ParamLeapArrayKeyPtrHashEq {
   public:
    static size_t hash(const ParamLeapArrayKeySharedPtr& key) {
      size_t result = key->interval_in_ms_;
      result = 31 * result + key->param_idx_;
      return result;
    }
    static bool equal(const ParamLeapArrayKeySharedPtr& k0,
                      const ParamLeapArrayKeySharedPtr& k1) {
      return k0->interval_in_ms_ == k1->interval_in_ms_ &&
             k0->param_idx_ == k1->param_idx_;
    }
  };

  ParamFlowRule() : ParamFlowRule("") {}
  explicit ParamFlowRule(const std::string& resource)
      : ParamFlowRule(resource, "") {}  // [P2]TODO: the role of limit_origin???
  explicit ParamFlowRule(const std::string& resource,
                         const std::string& limit_origin)
      : resource_(resource),
        metric_key_(std::make_shared<ParamLeapArrayKey>()) {}
  virtual ~ParamFlowRule() = default;

  const std::string& resource() const noexcept { return resource_; }
  ParamFlowMetricType metric_type() const noexcept { return metric_type_; }
  double threshold() const noexcept { return threshold_; }
  int param_idx() const noexcept { return param_idx_; }
  int interval_in_ms() const noexcept { return interval_in_ms_; }
  bool cluster_mode() const noexcept { return cluster_mode_; }
  ParamLeapArrayKeySharedPtr metric_key() const noexcept { return metric_key_; }

  std::vector<ParamFlowItem> param_item_list() const {
    return param_flow_item_list;
  }

  void set_resource(const std::string& resource) noexcept {
    this->resource_ = resource;
  }
  void set_metric_type(ParamFlowMetricType metric_type) noexcept {
    this->metric_type_ = metric_type;
  }
  void set_threshold(double threshold) noexcept {
    this->threshold_ = threshold;
  }
  void set_param_idx(int index) noexcept {
    this->param_idx_ = index;
    this->metric_key_->param_idx_ = index;
  }
  void set_interval_in_ms(int interval) noexcept {
    this->interval_in_ms_ = interval;
    this->metric_key_->interval_in_ms_ = interval;
  }
  void set_cluster_mode(bool cluster_mode) noexcept {
    this->cluster_mode_ = cluster_mode;
  }
  void set_param_flow_item_list(std::vector<ParamFlowItem>& list) noexcept {
    this->param_flow_item_list = std::move(list);
  }

  bool operator==(const ParamFlowRule& rule) const;
  // [P1]TODO: modify this method, consider all fields
  std::string ToString() const;

 private:
  ParamLeapArrayKeySharedPtr metric_key_;

  std::string resource_;                                        // resource
  ParamFlowMetricType metric_type_{ParamFlowMetricType::kQps};  // grade
  double threshold_ = 0;                                        // threshold
  int interval_in_ms_ = 1000;
  int param_idx_ = -1;

  std::vector<ParamFlowItem> param_flow_item_list;
  bool cluster_mode_ = false;  // clusterMode
};

using ParamFlowRuleSharedPtr = std::shared_ptr<ParamFlowRule>;
using ParamFlowRuleList = std::vector<ParamFlowRule>;
using ParamFlowRulePtrList = std::vector<ParamFlowRuleSharedPtr>;
using ParamFlowRulePtrListSharedPtr = std::shared_ptr<ParamFlowRulePtrList>;

struct ParamFlowRuleSharedPtrHashEq {
  static size_t hash(const ParamFlowRuleSharedPtr& rule) noexcept {
    std::size_t result = std::hash<std::string>{}(rule->resource());
    result = 31 * result + static_cast<int>(rule->metric_type());
    result = 31 * result + std::hash<double>{}(rule->threshold());
    result = 31 * result + std::hash<bool>{}(rule->cluster_mode());
    result = 31 * result + rule->interval_in_ms();
    // result = 31 * result + std::hash<T>{}(rule.param_flow_item_list());
    return result;
  }
  static bool equal(const ParamFlowRuleSharedPtr& r0,
                    const ParamFlowRuleSharedPtr& r1) noexcept {
    return r0->resource() == r1->resource() &&
           r0->metric_type() == r1->metric_type() &&
           r0->threshold() == r1->threshold() &&
           r0->param_idx() == r1->param_idx() &&
           r0->interval_in_ms() == r1->interval_in_ms() &&
           r0->cluster_mode() == r1->cluster_mode();
  }
};

class ParamFlowRuleHash {
 public:
  std::size_t operator()(const ParamFlowRule& rule) const noexcept {
    std::size_t result = std::hash<std::string>{}(rule.resource());
    result = 31 * result + static_cast<int>(rule.metric_type());
    result = 31 * result + std::hash<double>{}(rule.threshold());
    result = 31 * result + std::hash<bool>{}(rule.cluster_mode());
    result = 31 * result + static_cast<int>(rule.interval_in_ms());
    // result = 31 * result + std::hash<T>{}(rule.param_flow_item_list());
    return result;
  }
};

using ParamFlowRuleSet = std::unordered_set<ParamFlowRule, ParamFlowRuleHash>;

}  // namespace Param
}  // namespace Sentinel