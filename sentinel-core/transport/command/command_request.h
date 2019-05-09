#pragma once

#include <map>
#include <string>

namespace Sentinel {
namespace Transport {

class CommandRequest {
 public:
  const std::string& GetBody() const;
  CommandRequest& SetBody(const std::string& body);

  CommandRequest& AddParam(const std::string& key, const std::string& value);
  std::string GetParam(const std::string& key);
  std::string GetParam(const std::string& key, const std::string& defaultValue);
  const std::map<std::string, std::string>& GetParameters() const;

  CommandRequest& AddMetadata(const std::string& key, const std::string& value);
  std::string GetMetadata(const std::string& key) const;

 public:
  static const std::string kRequestTarget;

 private:
  std::map<std::string, std::string> metadata_;
  std::map<std::string, std::string> parameters_;
  std::string body_;
};

}  // namespace Transport
}  // namespace Sentinel
