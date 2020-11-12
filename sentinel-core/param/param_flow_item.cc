#include "sentinel-core/param/param_flow_item.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace Param {

bool ParamFlowItem::operator==(const ParamFlowItem& item) const noexcept {
  return param_type_ == item.param_type_ &&
         PubAnyEq(param_value_, item.param_value_) &&
         threshold_ == item.threshold_;
}

std::string ParamFlowItem::ToString() const {
  std::string typeName;
  if (IsInt(param_value_) || IsInt64(param_value_)) {
    typeName = "int";
  } else if (IsString(param_value_)) {
    typeName = "String";
  } else {
    typeName = "unknown";
  }
  return absl::StrFormat("ParamFlowItem{threshold=%d, type=%s}", threshold_,
                         typeName);
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