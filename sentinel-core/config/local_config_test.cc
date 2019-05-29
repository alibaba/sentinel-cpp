#include <cstdint>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/config/config_constants.h"
#include "sentinel-core/config/local_config.h"

namespace Sentinel {
namespace Config {

TEST(LocalConfigTest, TestResolveNormalAppName) {
  auto app_name = "test_app";
  setenv(Env::kAppNameKey, app_name, 1);
  LocalConfig config = LocalConfig::GetInstance();
  EXPECT_EQ(app_name, config.app_name());
  unsetenv(Env::kAppNameKey);
}

TEST(LocalConfigTest, TestGetIntOfInvalidValue) {
  LocalConfig config = LocalConfig::GetInstance();
  constexpr auto key = "some_key_bad";
  config.SetConfig(key, "a32");
  constexpr int32_t d = 32;
  EXPECT_EQ(d, config.GetInt32(key, d));
}

TEST(LocalConfigTest, TestGetIntOfNormalValue) {
  LocalConfig config = LocalConfig::GetInstance();
  constexpr auto key = "some_key_good";
  config.SetConfig(key, "64");
  constexpr int32_t d = 32;
  EXPECT_EQ(64, config.GetInt32(key, d));
}

}  // namespace Config
}  // namespace Sentinel