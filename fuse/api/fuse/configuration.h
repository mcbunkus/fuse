#pragma once

#include "fuse/result.hpp"
#include <string>

namespace fuse
{

class IConfiguration
{
  public:
    virtual Result<std::string> Get(const std::string &key) const = 0;
    virtual Result<> Set(const std::string &key, const std::string &value) = 0;

    virtual ~IConfiguration() = default;
};

class IConfigurable
{
  public:
    virtual Result<> configure(const IConfiguration &) = 0;
    virtual ~IConfigurable() = default;
};

} // namespace fuse
