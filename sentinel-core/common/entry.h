#pragma once

#include <chrono>
#include <memory>

#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {

class Entry {
 public:
  explicit Entry(ResourceWrapperSharedPtr&& resource)
      : resource_(std::move(resource)),
        create_time_(Utils::TimeUtils::CurrentTimeMillis()) {}
  explicit Entry(const ResourceWrapperSharedPtr& resource)
      : resource_(resource),
        create_time_(Utils::TimeUtils::CurrentTimeMillis()) {}

  virtual ~Entry() = default;

  virtual void Exit(int count);
  virtual void Exit();
  ResourceWrapperSharedPtr GetResourceWrapper() const { return resource_; }
  std::chrono::milliseconds GetCreateTime() const { return create_time_; }
  Stat::NodePtr GetCurrentNode() const { return cur_node_; }
  Stat::NodePtr GetOriginNode() const { return origin_node_; }
  bool HasError() const { return !error_.empty(); };

  void SetError(const std::string message) { error_ = message; }
  void SetCurNode(const Stat::NodePtr node) { cur_node_ = node; }
  void SetOriginNode(const Stat::NodePtr node) { origin_node_ = node; }

 private:
  bool is_exit_{false};
  const ResourceWrapperSharedPtr resource_;
  const std::chrono::milliseconds create_time_;

  std::string error_{};
  Stat::NodePtr cur_node_;
  Stat::NodePtr origin_node_;
};

using EntrySharedPtr = std::shared_ptr<Entry>;

}  // namespace Sentinel
