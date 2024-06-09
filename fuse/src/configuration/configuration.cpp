#include "fuse/configuration/configuration.hpp"

namespace fuse
{

namespace configuration
{

Section::Section(const std::string &name) : m_name{name} {}

std::string Section::Name() const { return m_name; }

bool Section::Has(const std::string &key)
{
    return m_keyvals.find(key) != m_keyvals.end();
}

bool Section::HasSection(const std::string &name)
{
    return m_subSections.find(name) != m_subSections.end();
}

void Section::Set(const std::string &key, const std::string &value)
{
    m_keyvals[key] = value;
}

void Section::Add(std::shared_ptr<Section> section)
{
    m_subSections[section->Name()] = section;
}

Result<std::string> Section::Get(const std::string &key)
{
    if (!Has(key))
    {
        return Err{m_name + " doesn't contain " + key};
    }

    try
    {
        return m_keyvals.at(key);
    }
    catch (const std::exception &e)
    {

        return Err{std::string("unexpected error occurred retrieving key: ") +
                   e.what()};
    }
}

Result<Section> Section::GetSection(const std::string &name)
{
    if (!HasSection(name))
    {
        return Err{m_name + " doesn't contain a section named " + name};
    }

    try
    {
        return *m_subSections.at(name);
    }
    catch (const std::exception &e)
    {
        return Err{
            std::string("unexpected error occurred retrieving section: ") +
            e.what()};
    }
}
} // namespace configuration

} // namespace fuse
