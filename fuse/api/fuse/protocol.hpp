#pragma once

#include "fuse/message.hpp"

namespace fuse {

class IProtocol {
public:
  virtual int Send(const IMessage &) = 0;
  virtual int Receive(IMessage &) = 0;
};

} // namespace fuse

// Handle for loading the IProtocol from an implementation
extern "C" fuse::IProtocol *GetProtocol();
