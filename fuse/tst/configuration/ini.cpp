#include "fuse/configuration/ini.hpp"
#include "fuse/result.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

using namespace fuse;

int main(int argc, char *argv[])
{
    std::stringstream ss;

    ss << "[foo]" << '\n';
    ss << "bar = baz" << '\n';

    configuration::Ini config;

    Result<> res = config.Load(ss);

    if (res.isErr())
    {
        std::cerr << "failed to load config: " << res.error() << std::endl;
        return 1;
    }

    // this should return an error
    Result<std::string> value = config.Get("nonExistentKey");
    if (!value.isErr())
    {
        std::cerr << "I expected to see an error, but got an Ok: " << *value
                  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "got correct error response: " << value.error()
                  << std::endl;
    }

    Result<configuration::Section> foo = config.GetSection("foo");

    if (foo.isErr())
    {
        std::cerr << "expected to get foo section, got error instead: "
                  << foo.error() << std::endl;

        return 1;
    }

    Result<std::string> baz = foo->Get("bar");

    if (baz.isErr())
    {
        std::cerr << "expected to get baz from bar, but got error: "
                  << baz.error() << std::endl;
        return 1;
    }

    assert(*baz == "baz");

    std::cout << "should be baz: " << *baz << std::endl;

    return 0;
}
