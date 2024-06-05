#pragma once

#include "fuse/message.hpp"

namespace fuse {

class IProtocol {
public:
  virtual int Send(const IMessage &) = 0;
  virtual int Receive(IMessage &) = 0;
};

} // namespace fuse
