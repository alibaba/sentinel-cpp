#include <memory>
#include <string>

#include "sentinel-core/transport/command/handler/vo/statistic_node_vo.h"

namespace Sentinel {
namespace Transport {

std::shared_ptr<StatisticNodeVO> StatisticNodeVO::FromResourceNode(
    const std::string& name, const Stat::ClusterNodeSharedPtr& node) {
  if (node == nullptr) {
    return nullptr;
  }
  std::shared_ptr<StatisticNodeVO> vo = std::make_shared<StatisticNodeVO>();
  vo->set_resource(name);
  vo->set_thread_num(node->CurThreadNum());
  vo->set_pass_qps(node->PassQps());
  vo->set_block_qps(node->BlockQps());
  vo->set_total_qps(node->TotalQps());
  vo->set_avg_rt(node->AvgRt());
  vo->set_complete_qps(node->CompleteQps());
  vo->set_exception_qps(node->ExceptionQps());
  vo->set_pass_per_min(node->PassCountInMinute());
  vo->set_block_per_min(node->BlockCountInMinute());
  vo->set_total_per_min(node->TotalCountInMinute());
  vo->set_exception_per_min(node->ExceptionCountInMinute());
  vo->set_timestamp(Utils::TimeUtils::CurrentTimeMillis().count());
  return vo;
}

}  // namespace Transport
}  // namespace Sentinel