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
  LocalConfig config;
  config.Initialize();
  EXPECT_EQ(app_name, config.app_name());
  unsetenv(Env::kAppNameKey);
}

TEST(LocalConfigTest, TestResolveEmptyAppName) {
  unsetenv(Env::kAppNameKey);
  LocalConfig config;
  config.Initialize();
  EXPECT_EQ(kUnknownAppName, config.app_name());
}

TEST(LocalConfigTest, TestGetIntOfInvalidValue) {
  LocalConfig config;
  constexpr auto key = "some_key_bad";
  config.SetConfig(key, "a32");
  constexpr int32_t d = 32;
  EXPECT_EQ(d, config.GetInt(key, d));
}

TEST(LocalConfigTest, TestGetIntOfNormalValue) {
  LocalConfig config;
  constexpr auto key = "some_key_good";
  config.SetConfig(key, "64");
  constexpr int32_t d = 32;
  EXPECT_EQ(64, config.GetInt(key, d));
}

}  // namespace Config
}  // namespace Sentinel