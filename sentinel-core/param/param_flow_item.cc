#include "sentinel-core/param/param_flow_item.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace Param {

bool ParamFlowItem::operator==(const ParamFlowItem& item) const noexcept {
  return param_type_ == item.param_type_ &&
         PubAnyEq(param_value_, item.param_value_) &&
         threshold_ == item.threshold_;
}

int ParamFlowItem::HashCode() const {
  int code = 1;
  code = 31 * code + std::hash<std::string>{}(param_type_);
  code = 31 * code + PubAnyHash(param_value_);
  code = 31 * code + threshold_;
  return code;
}

std::string ParamFlowItem::ToString() const {
  return absl::StrFormat("ParamFlowItem{threshold=%d, type=%s}", threshold_,
                         param_value_.type().name());
}

ParamFlowItemList::ParamFlowItemList(
    std::initializer_list<ParamFlowItem> args) {
  for (const auto& arg : args) {
    this->push_back(arg);
  }
}

bool ParamFlowItemList::operator==(const ParamFlowItemList& list) const
    noexcept {
  if (this->size() != list.size()) {
    return false;
  }
  for (int i = 0; i < this->size(); i++) {
    if (!(list[i] == this->at(i))) {
      return false;
    }
  }
  return true;
}

int ParamFlowItemList::HashCode() const noexcept {
  int code = 1;
  for (const auto& item : *this) {
    code = 31 * code + item.HashCode();
  }
  return code;
}

std::string ParamFlowItemList::ToString() const {
  std::string str("<");
  for (const auto& item : *this) {
    str += item.ToString();
    str += ",";
  }
  str += ">";
  return str;
}

}  // namespace Param
}  // namespace Sentinel