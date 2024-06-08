#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace fuse
{
namespace channel
{

/**
 * Sender allows a thread to send a message to a Receiver
 */
template <typename T> class Sender;

/**
 * Receiver receives messages sent by Sender
 */
template <typename T> class Receiver;

/**
 * Channel returns a pair of Sender and Receiver, threads can use Sender to send
 * messages to the paired Receiver. This is meant to mimic Rust's
 * std::sync::mpsc::channel function
 */
template <typename T> std::pair<Sender<T>, Receiver<T>> Channel();

/**
 * SharedState is "private", and is shared between a paired Sender and Receiver
 * to handle the actual sending/receiving
 */
template <typename T> class SharedState
{

  private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<T> m_queue;

    std::atomic_bool m_closed = false;

  public:
    void send(T value)
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_queue.push(std::move(value));
        lock.unlock();
        m_cond.notify_one();
    }

    std::optional<T> receive()
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_cond.wait(lock, [this] { return !m_queue.empty() || m_closed; });
        if (m_queue.empty())
        {
            return std::nullopt;
        }
        T value = std::move(m_queue.front());
        m_queue.pop();
        return value;
    }

    void close()
    {
        m_closed = true;
        m_cond.notify_all();
    }
};

template <typename T> class Sender
{

    std::shared_ptr<SharedState<T>> m_state;

  public:
    explicit Sender(std::shared_ptr<SharedState<T>> state)
        : m_state{std::move(state)}
    {
    }

    void send(T value) const { m_state->send(std::move(value)); }
    void close() const { m_state->close(); }
};

template <typename T> class Receiver
{
    std::shared_ptr<SharedState<T>> m_state;

  public:
    explicit Receiver(std::shared_ptr<SharedState<T>> state)
        : m_state{std::move(state)}
    {
    }

    std::optional<T> receive() { return m_state->receive(); }
};

/**
 * USE THIS
 *
 * Channel returns a paired Sender and Receiver that threads can use to
 * communicate with one another. This is similar conceptually to a channel in
 * Golang, but I like Rust's approach because it makes a clearer distinction
 * between who's sending and who's receiving.
 *
 * See fuse/tst/channel.cpp for an example on how to use Channel
 */
template <typename T> std::pair<Sender<T>, Receiver<T>> Channel()
{
    auto state = std::make_shared<SharedState<T>>();
    return {Sender<T>{state}, Receiver<T>{state}};
}

} // namespace channel
} // namespace fuse
