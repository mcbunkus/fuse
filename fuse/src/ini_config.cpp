
#include "ini_config.hpp"

namespace fuse
{
IniConfig::IniConfig(const std::string &filepath)
    : m_iniFile{new Poco::Util::IniFileConfiguration{filepath}}
{
}

Result<std::string> IniConfig::Get(const std::string &key) const
{
    if (!m_iniFile->has(key))
    {
        return Err("ini file does't contain a " + key + " key");
    }
    return Ok(m_iniFile->getString(key));
}
Result<> IniConfig::Set(const std::string &key, const std::string &value)
{
    m_iniFile->setString(key, value);
    return Ok();
}

} // namespace fuse
