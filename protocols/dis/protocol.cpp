
#include "protocol.hpp"
#include "fuse/result.hpp"

#include <sstream>

fuse::Result<int> DisProtocol::Send(const fuse::IMessage &msg)
{
    if (!m_recvdFrom)
    {
        return fuse::Ok(0);
    }

    // TODO: this might be confusing working with strings here, when in reality
    // we're just working with raw bytes. Maybe make a class that makes it more
    // obvious this is just a byte stream?
    std::ostringstream buffer;
    msg.Deserialize(buffer);

    std::string data = buffer.str();

    uint32_t nsent = 0;
    try
    {
        nsent = m_sock->sendTo(data.data(), data.size(), *m_recvdFrom);
    }
    catch (const Poco::Exception &e)
    {
        return fuse::Err(std::string("error sending message in DisProtocol: ") +
                         e.what());
    }

    if (nsent != data.size())
    {
        return fuse::Err("failed to send the full message in DisProtocol for "
                         "unknown reason: only sent " +
                         std::to_string(nsent) + " bytes");
    }

    return fuse::Ok((int)nsent);
}

// TODO: actually implement this
fuse::Result<int> DisProtocol::Receive(fuse::IMessage &msg)
{
    return fuse::Ok(42);
}

fuse::Result<> DisProtocol::configure(const fuse::IConfiguration &config)
{
    fuse::Result<std::string> ip = config.Get("ip");

    if (ip.isErr())
    {
        ip.error() += "failed to get IP address for socket in DisProtocol";
        return ip.error();
    }

    fuse::Result<std::string> port = config.Get("port");
    if (port.isErr())
    {
        port.error() += "failed to get port for socket in DisProtocol";
        return port.error();
    }

    Poco::Net::SocketAddress address{*ip + ":" + *port};
    m_sock = std::make_unique<Poco::Net::DatagramSocket>(address, true);

    return fuse::Ok();
}

extern "C" fuse::IProtocol *GetProtocol() { return new DisProtocol(); }
