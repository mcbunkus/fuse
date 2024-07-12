
FUSE (Flexible Uniform Simulation Exchanger) is a library intended to make integrating simulation protocols easier. It provides utility code, interfaces, and protocol implementations that enables users to more easily implement support for protocols such as GTISS, DIS, etc. Its goal is not to be a magical, drop in solution for every situation, but to provide you enough tooling to quickly hook up a model to different protocols. 

## Requirements

- Define interfaces for protocols to enable runtime polymorphism via `dlopen`
- Provide utility code for error handling, configuration, inter-thread communication, and safer multithreading
- Allow users to register hooks, or callbacks, that are executed when a specific message type is received
- Allow users to send concrete message types (OpenDIS messages, GTISS GDUs, etc)


## Design

Ideally, the architecture should be as simple as this:


![[FUSE Component.png]]

In practice, it probably won't be this simple. Each model works very differently, and each protocol is very different from one another. If you compare DIS and GTISS, you'll see there's analogous messages for starts, stops, entity states, etc. However, you'll also see each GTISS message contains different fields from their DIS counterparts. They don't map cleanly to one another.


### API

```cpp
namespace FUSE {

// Call this in a main somewhere as soon as possible
void Initialize(IProtocol *protocol);

// ====== Message handling ======

// Clients register callbacks to FUSE that are executed when FUSE
// receives that message type from its IProtocol implementation
template <typename T>
std::future<Result<>> OnReceive(std::function<Result<>(const T&)>);

// Overloaded implementations of Send that cover the basic message types
// clients can send. That way, they don't have to worry about serializing
// the messages, they can just create an EntityStatePDU, etc and pass it
// to Send.
void Send(const DIS::Pdu&);
void Send(const GTISS::GDU&);
// ... and so on
	
}
```

### Native messages

FUSE implementations will be required to operate on native messages. FUSE's predecessor (ESFI) defined its own set of messages called EMUs, which were meant to represent common message types. Starts, stops, entity states, etc. 

It's also possible that the model will have to account for the protocol it's talking to. For example, sensor models need to provide their radar parameters in an initialization message to OSF when running with GTISS. This doesn't exist in DIS. Additionally, sensors receive threat information tailored to their field of view from OSF in one big message. Contrast this with DIS, where each sensor get's every DIS entity's state which they have to filter out, and you can see it's difficult to combine these into common message types.

To make it easier to integrate into models, FUSE 



```cpp

using ProtocolCallback = std::function<void(const Message&)>;

class IProtocol {
public:
	virtual void Send(const Message&) = 0;
	virtual void OnReceive(std::type_index msgType, ProtocolCallback callback) = 0;
	virtual ~IProtocol() = default;
};

```