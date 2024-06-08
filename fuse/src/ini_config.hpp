#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "fuse/configuration.h"
#include <memory>

namespace fuse {
class IniConfig : public Configuration {

  Poco::AutoPtr<Poco::Util::IniFileConfiguration> m_iniFile;

public:
  IniConfig(const std::string &filepath);

  std::string Get(const std::string &key) override;
  void Set(const std::string &key, const std::string &value) override;

  ~IniConfig() = default;
};
} // namespace fuse
