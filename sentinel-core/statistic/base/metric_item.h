#pragma once

#include <memory>
#include <string>

namespace Sentinel {
namespace Stat {

class MetricItem;

using MetricItemSharedPtr = std::shared_ptr<MetricItem>;

class MetricItem {
 public:
  MetricItem() = default;

  static MetricItemSharedPtr FromThinString(const std::string& thin_str);
  static MetricItemSharedPtr FromFatString(const std::string& fat_str);

  const std::string& resource() const { return resource_; };
  int64_t timestamp() const { return timestamp_; };
  int64_t pass_qps() const { return pass_qps_; };
  int64_t block_qps() const { return block_qps_; };
  int64_t complete_qps() const { return complete_qps_; };
  int64_t exception_qps() const { return exception_qps_; };
  int64_t rt() const { return rt_; };

  void set_resource(const std::string& resource) { resource_ = resource; };
  void set_timestamp(int64_t t) { timestamp_ = t; };
  void set_pass_qps(int64_t p) { pass_qps_ = p; };
  void set_block_qps(int64_t b) { block_qps_ = b; };
  void set_complete_qps(int64_t c) { complete_qps_ = c; };
  void set_exception_qps(int64_t e) { exception_qps_ = e; };
  void set_rt(int64_t rt) { rt_ = rt; };

  std::string ToThinString() const;
  std::string ToFatString() const;

 private:
  std::string resource_;
  int64_t timestamp_;
  int64_t pass_qps_;
  int64_t block_qps_;
  int64_t complete_qps_;
  int64_t exception_qps_;
  int64_t rt_;
};  // MetricNode in Java version

}  // namespace Stat
}  // namespace Sentinel