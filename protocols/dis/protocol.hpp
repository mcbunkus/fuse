#include <memory>
#include <optional>

#include "fuse/protocol.hpp"

class DisProtocol : public fuse::IProtocol
{

    int m_sockfd;

  public:
    DisProtocol() = default;

    fuse::Result<int> Send(const fuse::IMessage &msg) override;
    fuse::Result<int> Receive(fuse::IMessage &msg) override;

    fuse::Result<>
    configure(const fuse::configuration::IConfiguration &config) override;

    ~DisProtocol() = default;
};

extern "C" fuse::IProtocol *GetProtocol();
