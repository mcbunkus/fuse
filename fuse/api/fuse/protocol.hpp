#pragma once

#include "fuse/configuration/configuration.hpp"
#include "fuse/message.hpp"
#include "fuse/result.hpp"

namespace fuse
{

class IProtocol : public configuration::IConfigurable
{
  public:
    virtual Result<int> Send(const IMessage &) = 0;
    virtual Result<int> Receive(IMessage &) = 0;

    virtual ~IProtocol() = default;
};

} // namespace fuse

// Handle for loading the IProtocol from an implementation
extern "C" fuse::IProtocol *GetProtocol();
