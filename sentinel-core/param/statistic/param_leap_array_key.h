#pragma once
#include <memory>

namespace Sentinel {
namespace Param {

class ParamLeapArrayKey {
 public:
  int32_t param_idx_ = -1;
  int32_t interval_in_ms_ = 1000;
  int32_t sample_count_ = 1;
  int32_t cache_size_;
  int32_t param_idx() const noexcept { return param_idx_; }
  int32_t interval_in_ms() const noexcept { return interval_in_ms_; }
  int32_t sample_count() const noexcept { return sample_count_; }
  int32_t cache_size() const noexcept { return cache_size_; }
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

}  // namespace Param
}  // namespace Sentinel
