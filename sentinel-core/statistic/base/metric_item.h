#pragma once

#include <memory>
#include <string>

namespace Sentinel {
namespace Stat {

class MetricItem;

using MetricItemSharedPtr = std::shared_ptr<MetricItem>;

class MetricItem {
 public:
  explicit MetricItem() = default;

  static MetricItemSharedPtr FromThinString(const std::string& thin_str);
  static MetricItemSharedPtr FromFatString(const std::string& fat_str);

  std::string& resource();
  long timestamp();
  long pass_qps();
  long block_qps();
  long complete_qps();
  long exception_qps();
  long rt();

  std::string ToThinString();
  std::string ToFatString();

 private:
  std::string resource_;
  long timestamp_;
  long pass_qps_;
  long block_qps_;
  long complete_qps_;
  long exception_qps_;
  long rt_;
};  // MetricNode in Java version

}  // namespace Stat
}  // namespace Sentinel