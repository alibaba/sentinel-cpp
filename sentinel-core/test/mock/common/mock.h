
#pragma once

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/rule.h"

namespace Sentinel {

class FakeRule : public Rule {
 public:
  FakeRule() = default;

  void set_fake_rule_data(const std::string& data) { fake_rule_data_ = data; }
  const std::string& fake_rule_data() const { return fake_rule_data_; }

  bool operator==(const FakeRule& other) const {
    return fake_rule_data_ == other.fake_rule_data();
  }

 private:
  std::string fake_rule_data_;
};

}  // namespace Sentinel
