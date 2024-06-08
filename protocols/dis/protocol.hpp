#include "Poco/Net/DatagramSocket.h"
#include "fuse/protocol.hpp"

class DisProtocol : public fuse::IProtocol {

  Poco::Net::DatagramSocket m_sock;

public:
  int Send(const fuse::IMessage &msg) override;
  int Receive(fuse::IMessage &msg) override;

  ~DisProtocol() = default;
};

extern "C" fuse::IProtocol *GetProtocol();
