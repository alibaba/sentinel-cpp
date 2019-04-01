#pragma once

#include <vector>

#include "sentinel-core/statistic/base/metric_bucket.h"

namespace Sentinel {
namespace Stat {

class Metric {
 public:
  virtual ~Metric() = default;

  virtual long Complete() = 0;

  /**
   * Get max success count.
   *
   * @return max success count
   */
  virtual long MaxComplete() = 0;

  /**
   * Get total exception count.
   *
   * @return exception count
   */
  virtual long Exception() = 0;

  /**
   * Get total block count.
   *
   * @return block count
   */
  virtual long Block() = 0;

  /**
   * Get total pass count. not include {@link #occupiedPass()}
   *
   * @return pass count
   */
  virtual long Pass() = 0;

  /**
   * Get total response time.
   *
   * @return total RT
   */
  virtual long Rt() = 0;

  /**
   * Get the minimal RT.
   *
   * @return minimal RT
   */
  virtual long MinRt() = 0;

  /**
   * Get aggregated metric nodes of all resources.
   *
   * @return metric node list of all resources
   */
  // virtual List<MetricNode> details() = 0;

  /**
   * Get the raw window array.
   *
   * @return window metric array
   */
  virtual std::vector<MetricBucket> Windows() = 0;

  /**
   * Add current exception count.
   *
   * @param n count to add
   */
  virtual void AddException(int n) = 0;

  /**
   * Add current block count.
   *
   * @param n count to add
   */
  virtual void AddBlock(int n) = 0;

  /**
   * Add current completed count.
   *
   * @param n count to add
   */
  virtual void AddComplete(int n) = 0;

  /**
   * Add current pass count.
   *
   * @param n count to add
   */
  virtual void AddPass(int n) = 0;

  /**
   * Add given RT to current total RT.
   *
   * @param rt RT
   */
  virtual void AddRt(long rt) = 0;

  /**
   * Get the sliding window length in seconds.
   *
   * @return the sliding window length
   */
  virtual double GetWindowIntervalInSec() = 0;
};

}  // namespace Stat
}  // namespace Sentinel