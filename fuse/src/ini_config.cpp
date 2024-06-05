
#include "ini_config.hpp"

namespace fuse {
IniConfig::IniConfig(const std::string &filepath)
    : m_iniFile{new Poco::Util::IniFileConfiguration{filepath}} {}

std::string IniConfig::Get(const std::string &key) {
  return m_iniFile->getString(key);
}
void IniConfig::Set(const std::string &key, const std::string &value) {
  m_iniFile->setString(key, value);
}

} // namespace fuse
