#include <chrono>
#include <iostream>
#include <thread>

#include "fuse/channel.hpp"

void producer(const fuse::channel::Sender<int> sender)
{
    for (int i = 0; i < 5; ++i)
    {
        sender.send(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    sender.close();
}

void consumer(fuse::channel::Receiver<int> receiver)
{
    while (std::optional<int> value = receiver.receive())
    {
        std::cout << "received: " << *value << "\n";
    }

    std::cout << "receiver closed" << std::endl;
}

int main(int argc, char *argv[])
{
    auto [sender, receiver] = fuse::channel::Channel<int>();

    std::thread producerThread = std::thread(producer, sender);
    std::thread consumerThread = std::thread(consumer, receiver);

    producerThread.join();
    consumerThread.join();

    return 0;
}
