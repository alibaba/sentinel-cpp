#pragma once

#include <string>
#include <unordered_map>

namespace Sentinel {
namespace Transport {

class CommandRequest {
 public:
  const std::string& body() const;
  CommandRequest& set_body(const std::string& body);

  CommandRequest& AddParam(const std::string& key, const std::string& value);
  std::string GetParam(const std::string& key) const;
  std::string GetParam(const std::string& key,
                       const std::string& defaultValue) const;
  const std::unordered_map<std::string, std::string>& GetParameters() const;

  CommandRequest& AddMetadata(const std::string& key, const std::string& value);
  std::string GetMetadata(const std::string& key) const;

 private:
  std::unordered_map<std::string, std::string> metadata_;
  std::unordered_map<std::string, std::string> parameters_;
  std::string body_;
};

constexpr char kRequestTarget[] = "command-target";

}  // namespace Transport
}  // namespace Sentinel
