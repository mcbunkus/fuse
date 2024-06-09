
#include "fuse/util.hpp"

namespace fuse
{
namespace util
{

static const std::string whitespace = " \t\n\r\f\v";

std::string TrimLeadingWhitespace(std::string input)
{
    input.erase(0, input.find_first_not_of(whitespace));
    return input;
}

std::string TrimTrailingWhitespace(std::string input)
{
    input.erase(input.find_last_not_of(whitespace) + 1);
    return input;
}

std::string TrimWhitespace(std::string input)
{
    return TrimLeadingWhitespace(TrimTrailingWhitespace(input));
}

} // namespace util
} // namespace fuse
