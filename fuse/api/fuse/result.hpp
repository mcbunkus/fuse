#pragma once
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility> // For std::forward
#include <variant>

namespace fuse
{

class Err
{
    std::string m_desc;

  public:
    Err(const std::string &desc) : m_desc{desc} {}

    // For user to add additional context from errors bubbling up from below
    void operator+=(const Err &e) { m_desc = e.m_desc + ": " + m_desc; }
    void operator+=(const std::string &desc) { m_desc = desc + ": " + m_desc; }

    std::string description() const { return m_desc; }

    friend std::ostream &operator<<(std::ostream &os, const Err &e)
    {
        os << e.m_desc;
        return os;
    }
};

// std::monostate is how you would represent "void" in a std::variant
template <typename T = std::monostate, typename E = Err> class Result
{
    // Variant to hold either the Ok value or the Err value
    std::variant<T, E> m_result;

  private:
    // Private constructor for the static factory methods
    template <typename... Args>
    Result(std::in_place_index_t<0>, Args &&...args)
        : m_result(std::in_place_index<0>, std::forward<Args>(args)...)
    {
    }

    template <typename... Args>
    Result(std::in_place_index_t<1>, Args &&...args)
        : m_result(std::in_place_index<1>, std::forward<Args>(args)...)
    {
    }

  public:
    Result(const T &value) : m_result{value} {}
    Result(T &&value) : m_result{std::move(value)} {}

    Result(const E &value) : m_result{value} {}
    Result(E &&value) : m_result{std::move(value)} {}

    // Static for Ok and Err
    static Result Ok(T value = T())
    {
        return Result(std::in_place_index<0>, std::move(value));
    }

    static Result Err(E error)
    {
        return Result(std::in_place_index<1>, std::move(error));
    }

    // Check if the result is Ok
    bool isOk() const { return m_result.index() == 0; }

    // Check if the result is Err
    bool isErr() const { return m_result.index() == 1; }

    // Get the value if the result is Ok, throws if not
    T &unwrap()
    {
        if (isOk())
        {
            return std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to unwrap an Err value");
        }
    }

    const T &unwrap() const
    {
        if (isOk())
        {
            return std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to unwrap an Err value");
        }
    }

    // Get the error if the result is Err, throws if not
    E &error()
    {
        if (isErr())
        {
            return std::get<1>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to unwrap an Ok value");
        }
    }

    const E &error() const
    {
        if (isErr())
        {
            return std::get<1>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to unwrap an Ok value");
        }
    }

    // Operator-> to access the Ok value's methods
    T *operator->()
    {
        if (isOk())
        {
            return &std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to access an Err value");
        }
    }

    const T *operator->() const
    {
        if (isOk())
        {
            return &std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to access an Err value");
        }
    }

    // Operator* to access the Ok value directly
    T &operator*()
    {
        if (isOk())
        {
            return std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to access an Err value");
        }
    }

    const T &operator*() const
    {
        if (isOk())
        {
            return std::get<0>(m_result);
        }
        else
        {
            throw std::runtime_error("Attempted to access an Err value");
        }
    }
};

// Convenience function to create an Ok result
template <typename T = std::monostate, typename E = Err>
Result<T, E> Ok(T value = T())
{
    return Result<T, E>::Ok(std::move(value));
}

} // namespace fuse
