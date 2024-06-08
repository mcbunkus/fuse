#pragma once

#include <string>
namespace fuse {

class Configuration {
public:
  virtual std::string Get(const std::string &key) = 0;
  virtual void Set(const std::string &key, const std::string &value) = 0;

  virtual ~Configuration() = default;
};

} // namespace fuse
