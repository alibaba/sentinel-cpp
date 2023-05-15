#include <memory>
#include <string>

#include "sentinel-core/statistic/node/cluster_node.h"

namespace Sentinel {
namespace Transport {

class StatisticNodeVO {
 public:
  StatisticNodeVO() = default;
  ~StatisticNodeVO() = default;

  static std::shared_ptr<StatisticNodeVO> FromResourceNode(
      const std::string& name, const Stat::ClusterNodeSharedPtr& node);

  const std::string& id() const { return id_; }
  const std::string& parent_id() const { return parent_id_; }
  const std::string& resource() const { return resource_; };
  int32_t thread_num() const { return thread_num_; }
  int64_t pass_qps() const { return pass_qps_; }
  int64_t block_qps() const { return block_qps_; }
  int64_t total_qps() const { return total_qps_; }
  int64_t complete_qps() const { return complete_qps_; }
  int64_t avg_rt() const { return avg_rt_; }
  int64_t exception_qps() const { return exception_qps_; }
  int64_t pass_per_min() const { return pass_per_min_; }
  int64_t block_per_min() const { return block_per_min_; }
  int64_t exception_per_min() const { return exception_per_min_; }
  int64_t total_per_min() const { return total_per_min_; }
  int64_t timestamp() const { return timestamp_; }

  void set_id(const std::string& id) { id_ = id; }
  void set_parent_id(const std::string& id) { parent_id_ = id; }
  void set_resource(const std::string& resource) { resource_ = resource; }
  void set_thread_num(int32_t v) { thread_num_ = v; }
  void set_pass_qps(int64_t v) { pass_qps_ = v; }
  void set_block_qps(int64_t v) { block_qps_ = v; }
  void set_total_qps(int64_t v) { total_qps_ = v; }
  void set_complete_qps(int64_t v) { complete_qps_ = v; }
  void set_avg_rt(int64_t v) { avg_rt_ = v; }
  void set_exception_qps(int64_t v) { exception_qps_ = v; }
  void set_pass_per_min(int64_t v) { pass_per_min_ = v; }
  void set_block_per_min(int64_t v) { block_per_min_ = v; }
  void set_exception_per_min(int64_t v) { exception_per_min_ = v; }
  void set_total_per_min(int64_t v) { total_per_min_ = v; }
  void set_timestamp(int64_t v) { timestamp_ = v; }

 private:
  std::string id_;
  std::string parent_id_;
  std::string resource_;
  int32_t thread_num_;
  int64_t pass_qps_;
  int64_t block_qps_;
  int64_t total_qps_;
  int64_t complete_qps_;
  int64_t avg_rt_;
  int64_t exception_qps_;
  int64_t pass_per_min_;
  int64_t block_per_min_;
  int64_t exception_per_min_;
  int64_t total_per_min_;

  int64_t timestamp_;
};

}  // namespace Transport
}  // namespace Sentinel