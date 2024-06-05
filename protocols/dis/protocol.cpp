#include "protocol.hpp"

extern "C" fuse::IProtocol *GetProtocol() { return new DisProtocol(); }
