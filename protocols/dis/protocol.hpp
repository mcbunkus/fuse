#include <memory>
#include <optional>

#include "Poco/Net/DatagramSocket.h"
#include "fuse/protocol.hpp"

class DisProtocol : public fuse::IProtocol
{

    std::unique_ptr<Poco::Net::DatagramSocket> m_sock;
    std::optional<Poco::Net::SocketAddress> m_recvdFrom;

  public:
    DisProtocol() = default;

    fuse::Result<int> Send(const fuse::IMessage &msg) override;
    fuse::Result<int> Receive(fuse::IMessage &msg) override;

    fuse::Result<> configure(const fuse::IConfiguration &config) override;

    ~DisProtocol() = default;
};

extern "C" fuse::IProtocol *GetProtocol();
