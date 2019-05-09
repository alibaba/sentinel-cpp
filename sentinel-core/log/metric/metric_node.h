#pragma once

#include <stdint.h>
#include <string>

namespace Sentinel {
namespace Metric {

class MetricNode {
 public:
  /**
   * To formatting string. All "|" in {@link #resource} will be replaced with
   * "_", format is: <br/>
   * <code>
   * timestamp|resource|passQps|blockQps|successQps|exceptionQps|rt
   * |occupiedPassQps
   * </code>
   *
   * @return string format of this.
   */
  std::string ToThinString();

  /**
   * To formatting string. All "|" in {@link MetricNode#resource} will be
   * replaced with "_", format is: <br/>
   * <code>
   * timestamp|yyyy-MM-dd
   * HH:mm:ss|resource|passQps|blockQps|successQps|exceptionQps|rt|occupiedPassQps\n
   * </code>
   *
   * @return string format of this.
   */
  std::string ToFatString();

 public:
  /**
   * Parse {@link MetricNode} from thin string, see {@link #toThinString()}
   *
   * @param line
   * @return
   */
  static MetricNode FromThinString(const std::string &line);

  /**
   * Parse {@link MetricNode} from fat string, see {@link #toFatString()}
   *
   * @param line
   * @return the {@link MetricNode} parsed.
   */
  static MetricNode FromFatString(const std::string &line);

 public:
  int64_t GetTimestamp() const;
  void SetTimestamp(int64_t timestamp_);
  int64_t GetPassQps() const;
  void SetPassQps(int64_t pass_qps_);
  int64_t GetBlockQps() const;
  void SetBlockQps(int64_t block_qps_);
  int64_t GetSuccessQps() const;
  void SetSuccessQps(int64_t success_qps_);
  int64_t GetExceptionQps() const;
  void SetExceptionQps(int64_t exception_qps_);
  int64_t GetRt() const;
  void SetRt(int64_t rt_);
  int64_t GetOccupiedPassQps() const;
  void SetOccupiedPassQps(int64_t occupied_pass_qps_);
  const std::string &GetResource() const;
  void SetResource(const std::string &resource_);

 private:
  int64_t timestamp_;
  int64_t pass_qps_;
  int64_t block_qps_;
  int64_t success_qps_;
  int64_t exception_qps_;
  int64_t rt_;
  int64_t occupied_pass_qps_;
  std::string resource_;
};

}  // namespace Metric
}  // namespace Sentinel