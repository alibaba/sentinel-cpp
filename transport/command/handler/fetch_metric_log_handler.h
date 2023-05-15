#pragma once

#include <memory>
#include <vector>

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_searcher.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/transport/command/command_handler.h"

namespace Sentinel {
namespace Transport {

class FetchMetricLogCommandHandler : public CommandHandler {
 public:
  FetchMetricLogCommandHandler() : CommandHandler("metric") {
    const std::string& app_name = Config::LocalConfig::GetInstance().app_name();
    searcher_ = std::make_unique<Log::MetricSearcher>(
        Log::LogBase::GetLogBaseDir(),
        Log::MetricWriter::FormSelfMetricFileName(app_name));
  }

  virtual ~FetchMetricLogCommandHandler() = default;
  CommandResponsePtr Handle(const CommandRequest& request) override;

 private:
  std::unique_ptr<Log::MetricSearcher> searcher_;
};

}  // namespace Transport
}  // namespace Sentinel