#pragma once

#include <iostream>

namespace fuse {

class IMessage {
public:
  virtual int Serialize(std::istream &) = 0;
  virtual int Deserialize(std::ostream &) = 0;
};

} // namespace fuse
