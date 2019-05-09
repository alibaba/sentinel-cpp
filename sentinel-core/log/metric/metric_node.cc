#include "metric_node.h"

#include <sstream>

#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"
//#include "sentinel-core/log/record_log.h"

namespace Sentinel {
namespace Metric {

std::string MetricNode::ToThinString() {
  std::stringstream ss;
  ss << timestamp_ << "|"
      << absl::StrReplaceAll(resource_, {{"|", "_"}}) << "|"
      << pass_qps_ << "|"
      << block_qps_ << "|"
      << success_qps_ << "|"
      << exception_qps_ << "|"
      << rt_ << "|"
      << occupied_pass_qps_;
  return ss.str();
}


std::string MetricNode::ToFatString() {
  std::stringstream ss;
  ss << timestamp_ << "|"
     << absl::StrReplaceAll(resource_, {{"|", "_"}}) << "|"
     << pass_qps_ << "|"
     << block_qps_ << "|"
     << success_qps_ << "|"
     << exception_qps_ << "|"
     << rt_ << "|"
     << occupied_pass_qps_ << "\n";

  return ss.str();
}


#define PARSE_AND_SET_INT_FIELD(SETTER, STR_VAL)              \
  int64_t SETTER##val = 0;                                    \
  if (absl::SimpleAtoi(STR_VAL, &SETTER##val)) {              \
    node.SETTER(SETTER##val);                                 \
  } else {                                                    \
    return node;                                              \
  }

MetricNode MetricNode::FromFatString(const std::string &line) {
  MetricNode node;
  std::vector<std::string> strs = absl::StrSplit(line, '|');
  if (strs.size() < 8) {
    //Sentinel::Log::RecordLog::Error(std::string("FromFatString size < 8, line:")
     //                                   + line);
    return node;
  }

  PARSE_AND_SET_INT_FIELD(SetTimestamp, strs[0]);
  node.SetResource(strs[1]);
  PARSE_AND_SET_INT_FIELD(SetPassQps, strs[2]);
  PARSE_AND_SET_INT_FIELD(SetBlockQps, strs[3]);
  PARSE_AND_SET_INT_FIELD(SetSuccessQps, strs[4]);
  PARSE_AND_SET_INT_FIELD(SetExceptionQps, strs[5]);
  PARSE_AND_SET_INT_FIELD(SetRt, strs[6]);

  if (strs.size() == 8) {
    PARSE_AND_SET_INT_FIELD(SetOccupiedPassQps, strs[7]);
  }

  return node;
}


int64_t MetricNode::GetTimestamp() const {
  return timestamp_;
}
void MetricNode::SetTimestamp(int64_t timestamp_) {
  MetricNode::timestamp_ = timestamp_;
}
int64_t MetricNode::GetPassQps() const {
  return pass_qps_;
}
void MetricNode::SetPassQps(int64_t pass_qps_) {
  MetricNode::pass_qps_ = pass_qps_;
}
int64_t MetricNode::GetBlockQps() const {
  return block_qps_;
}
void MetricNode::SetBlockQps(int64_t block_qps_) {
  MetricNode::block_qps_ = block_qps_;
}
int64_t MetricNode::GetSuccessQps() const {
  return success_qps_;
}
void MetricNode::SetSuccessQps(int64_t success_qps_) {
  MetricNode::success_qps_ = success_qps_;
}
int64_t MetricNode::GetExceptionQps() const {
  return exception_qps_;
}
void MetricNode::SetExceptionQps(int64_t exception_qps_) {
  MetricNode::exception_qps_ = exception_qps_;
}
int64_t MetricNode::GetRt() const {
  return rt_;
}
void MetricNode::SetRt(int64_t rt_) {
  MetricNode::rt_ = rt_;
}
int64_t MetricNode::GetOccupiedPassQps() const {
  return occupied_pass_qps_;
}
void MetricNode::SetOccupiedPassQps(int64_t occupied_pass_qps_) {
  MetricNode::occupied_pass_qps_ = occupied_pass_qps_;
}
const std::string &MetricNode::GetResource() const {
  return resource_;
}
void MetricNode::SetResource(const std::string &resource_) {
  MetricNode::resource_ = resource_;
}

} // namespace Sentinel
} // namespace Metric