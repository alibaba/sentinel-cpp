#pragma once

#include <chrono>
#include <memory>

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {

class Entry {
 public:
  explicit Entry(const ResourceWrapperSharedPtr& resource,
                 const EntryContextSharedPtr& context)
      : resource_(resource),
        context_(context),
        create_time_(Utils::TimeUtils::CurrentTimeMillis()) {}

  virtual ~Entry() = default;

  friend class EntryResult;
  friend class SphU;

  ResourceWrapperSharedPtr resource() const { return resource_; }
  std::chrono::milliseconds create_time() const { return create_time_; }
  EntryContextSharedPtr context() const { return context_; }
  Stat::NodeSharedPtr cur_node() const { return cur_node_; }
  Stat::NodeSharedPtr origin_node() const { return origin_node_; }
  bool exited() const { return exited_; }
  bool HasError() const { return !error_.empty(); };

  void set_error(const std::string& message) { error_ = message; }
  void set_cur_node(const Stat::NodeSharedPtr& node) { cur_node_ = node; }
  void set_origin_node(const Stat::NodeSharedPtr& node) { origin_node_ = node; }

 private:
  const ResourceWrapperSharedPtr resource_;
  EntryContextSharedPtr context_;
  const std::chrono::milliseconds create_time_;

  bool exited_{false};
  std::string error_{};
  Stat::NodeSharedPtr cur_node_;
  Stat::NodeSharedPtr origin_node_;
};

using EntrySharedPtr = std::shared_ptr<Entry>;

}  // namespace Sentinel
