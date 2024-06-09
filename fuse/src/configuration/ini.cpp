#include <cstring>
#include <fstream>
#include <string>

#include "fuse/configuration/ini.hpp"
#include "fuse/result.hpp"
#include "fuse/util.hpp"

namespace fuse
{
namespace configuration
{

Ini::Ini() { m_root = std::make_shared<Section>(""); }

Result<> Ini::Parse(const std::string &filepath)
{
    std::ifstream file{filepath};

    if (!file.is_open())
    {
        return Err{"unable to open " + filepath + ": " + std::strerror(errno)};
    }

    return Load(file);
}

Result<> Ini::Load(std::istream &stream)
{

    std::string line = "";
    std::shared_ptr<Section> currentSection = m_root;

    try
    {

        while (std::getline(stream, line))
        {
            line = line.substr(0, line.find(";")); // ignore comments
            line = util::TrimWhitespace(line);

            if (line.empty())
            {
                continue;
            }

            if (line.front() == '[' && line.back() == ']')
            {
                std::string sectionName = line.substr(1, line.size() - 2);
                std::shared_ptr<Section> newSection =
                    std::make_shared<Section>(sectionName);

                currentSection->Add(newSection);
                currentSection = newSection;
            }

            auto delimpos = line.find('=');
            if (delimpos == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, delimpos);
            std::string value = line.substr(delimpos + 1);

            key = util::TrimWhitespace(key);
            value = util::TrimWhitespace(value);

            currentSection->Set(key, value);
        }
    }
    catch (const std::exception &e)
    {
        return Err{std::string{"error loading INI config: "} + e.what()};
    }

    return Ok();
}

Result<std::string> Ini::Get(const std::string &key)
{
    return m_root->Get(key);
}

Result<Section> Ini::GetSection(const std::string &name)
{
    return m_root->GetSection(name);
}

} // namespace configuration
} // namespace fuse
