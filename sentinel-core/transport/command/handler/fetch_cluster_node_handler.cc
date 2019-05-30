#include <memory>
#include <unordered_map>
#include <vector>

#include "external/com_github_nlohmann_json/file/nlohmann/json.hpp"

#include "sentinel-core/statistic/node/resource_node_storage.h"
#include "sentinel-core/transport/command/handler/fetch_cluster_node_handler.h"
#include "sentinel-core/transport/command/handler/vo/statistic_node_vo.h"

namespace Sentinel {
namespace Transport {

nlohmann::json ConvertNodeVoToJson(
    const std::shared_ptr<StatisticNodeVO>& node) {
  return nlohmann::json{{"id", node->id()},
                        {"parentId", node->parent_id()},
                        {"resource", node->resource()},
                        {"threadNum", node->thread_num()},
                        {"passQps", node->pass_qps()},
                        {"blockQps", node->block_qps()},
                        {"totalQps", node->total_qps()},
                        {"averageRt", node->avg_rt()},
                        {"successQps", node->complete_qps()},
                        {"exceptionQps", node->exception_qps()},
                        {"oneMinutePass", node->pass_per_min()},
                        {"oneMinuteBlock", node->block_per_min()},
                        {"oneMinuteException", node->exception_per_min()},
                        {"oneMinuteTotal", node->total_per_min()},
                        {"timestamp", node->timestamp()}};
}

CommandResponseSharedPtr FetchClusterNodeCommandHandler::Handle(
    const CommandRequest& request) {
  std::string type = request.GetParam("type");
  std::vector<std::shared_ptr<StatisticNodeVO>> vec;

  const auto& node_map = Stat::ResourceNodeStorage::GetInstance().GetNodeMap();
  for (const auto& e : node_map) {
    if (type == "notZero") {
      auto node = e.second;
      if (node != nullptr && node->TotalCountInMinute() > 0) {
        vec.emplace_back(StatisticNodeVO::FromResourceNode(e.first, node));
      }
    } else {
      auto vo = StatisticNodeVO::FromResourceNode(e.first, e.second);
      if (vo != nullptr) {
        vec.emplace_back(std::move(vo));
      }
    }
  }

  auto j = nlohmann::json::array();
  for (const auto& v : vec) {
    j.push_back(ConvertNodeVoToJson(v));
  }
  return CommandResponse::OfSuccess(j.dump());
}

}  // namespace Transport
}  // namespace Sentinel