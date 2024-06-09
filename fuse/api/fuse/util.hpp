#pragma once

#include <string>

namespace fuse
{

namespace util
{

std::string TrimLeadingWhitespace(std::string input);
std::string TrimTrailingWhitespace(std::string input);
std::string TrimWhitespace(std::string input);

} // namespace util
} // namespace fuse
