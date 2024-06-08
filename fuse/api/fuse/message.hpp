#pragma once

#include "fuse/result.hpp"
#include <iostream>

namespace fuse
{

class IMessage
{
  public:
    virtual Result<int> Serialize(std::istream &) = 0;
    virtual Result<int> Deserialize(std::ostream &) const = 0;

    virtual ~IMessage() = default;
};

} // namespace fuse
