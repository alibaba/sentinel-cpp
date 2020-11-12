#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "absl/types/any.h"
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/rule.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/param/param_flow_item.h"
#include "sentinel-core/param/param_flow_rule_constants.h"
#include "tbb/concurrent_hash_map.h"

namespace Sentinel {
namespace Param {

using HotItemsMap = std::unordered_map<
    absl::any, int32_t, std::function<size_t(const absl::any& any)>,
    std::function<bool(const absl::any& any1, const absl::any& any2)>>;
using HotItemsMapSharedPtr = std::shared_ptr<HotItemsMap>;

// Deep copy safe
class ParamFlowRule : public Rule {
 public:
  class ParamLeapArrayKey {
   public:
    int32_t param_idx_ = -1;
    int32_t interval_in_ms_ = 1000;
    int32_t sample_count_ = 1;
    int32_t cache_size_ = DEFAULT_CACHE_SIZE;
  };
  using ParamLeapArrayKeySharedPtr = std::shared_ptr<ParamLeapArrayKey>;

  class ParamLeapArrayKeyPtrHashEq {
   public:
    static size_t hash(const ParamLeapArrayKeySharedPtr& key) {
      if (!key) {
        return 0;
      }
      size_t result = key->interval_in_ms_;
      result = 31 * result + key->sample_count_;
      result = 31 * result + key->param_idx_;
      result = 31 * result + key->cache_size_;
      return result;
    }
    static bool equal(const ParamLeapArrayKeySharedPtr& k0,
                      const ParamLeapArrayKeySharedPtr& k1) {
      if (!k0 && !k1) {
        return true;
      } else if (!k0 || !k1) {
        return false;
      }
      return k0->interval_in_ms_ == k1->interval_in_ms_ &&
             k0->sample_count_ == k1->sample_count_ &&
             k0->param_idx_ == k1->param_idx_ &&
             k0->cache_size_ == k1->cache_size_;
    }
  };

  ParamFlowRule() : ParamFlowRule("") {}
  explicit ParamFlowRule(const std::string& resource)
      : resource_(resource),
        metric_key_(std::make_shared<ParamLeapArrayKey>()),
        hot_items_(std::make_shared<HotItemsMap>(1, PubAnyHash, PubAnyEq)) {}
  virtual ~ParamFlowRule() = default;

  const std::string& resource() const noexcept { return resource_; }
  ParamFlowMetricType metric_type() const noexcept { return metric_type_; }
  double threshold() const noexcept { return threshold_; }
  int32_t param_idx() const noexcept { return param_idx_; }
  int32_t cache_size() const noexcept { return cache_size_; }
  int32_t interval_in_ms() const noexcept { return interval_in_ms_; }
  int32_t sample_count() const noexcept { return sample_count_; }
  bool cluster_mode() const noexcept { return cluster_mode_; }
  HotItemsMapSharedPtr hot_items() const noexcept { return hot_items_; }
  ParamLeapArrayKeySharedPtr metric_key() const noexcept { return metric_key_; }

  ParamFlowItemList param_item_list() const { return param_flow_item_list_; }

  void set_resource(const std::string& resource) noexcept {
    this->resource_ = resource;
  }
  void set_metric_type(ParamFlowMetricType metric_type) noexcept {
    this->metric_type_ = metric_type;
  }
  void set_threshold(double threshold) noexcept {
    this->threshold_ = threshold;
  }
  void set_param_idx(int32_t index) noexcept {
    this->param_idx_ = index;
    this->metric_key_->param_idx_ = index;
  }
  void set_cache_size(int32_t cache_size) noexcept {
    this->cache_size_ = cache_size;
    this->metric_key_->cache_size_ = cache_size;
  }
  void set_sample_count(int32_t sample_count) noexcept {
    this->sample_count_ = sample_count;
    this->metric_key_->sample_count_ = sample_count;
  }
  void set_interval_in_ms(int32_t interval) noexcept {
    this->interval_in_ms_ = interval;
    this->metric_key_->interval_in_ms_ = interval;
  }
  void set_cluster_mode(bool cluster_mode) noexcept {
    this->cluster_mode_ = cluster_mode;
  }
  void set_param_flow_item_list(ParamFlowItemList&& list) noexcept {
    this->param_flow_item_list_ = std::move(list);
  }

  void FillExceptionFlowItems() const;
  bool operator==(const ParamFlowRule& rule) const;
  std::string ToString() const;
  const static int32_t DEFAULT_CACHE_SIZE = 200;

 private:
  ParamLeapArrayKeySharedPtr metric_key_;

  std::string resource_;  // resource
  int32_t param_idx_ = -1;
  ParamFlowMetricType metric_type_{ParamFlowMetricType::kQps};  // grade
  double threshold_ = 0;                                        // threshold
  int32_t interval_in_ms_ = 1000;
  int32_t sample_count_ = 1;
  int32_t cache_size_ = DEFAULT_CACHE_SIZE;

  mutable ParamFlowItemList param_flow_item_list_;
  mutable HotItemsMapSharedPtr hot_items_;

  bool cluster_mode_ = false;  // clusterMode
};

using ParamFlowRuleSharedPtr = std::shared_ptr<ParamFlowRule>;
using ParamFlowRuleList = std::vector<ParamFlowRule>;
using ParamFlowRulePtrList = std::vector<ParamFlowRuleSharedPtr>;
using ParamFlowRulePtrListSharedPtr = std::shared_ptr<ParamFlowRulePtrList>;

}  // namespace Param
}  // namespace Sentinel