#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "fuse/configuration.h"

namespace fuse
{
class IniConfig : public IConfiguration
{

    Poco::AutoPtr<Poco::Util::IniFileConfiguration> m_iniFile;

  public:
    IniConfig(const std::string &filepath);

    Result<std::string> Get(const std::string &key) const override;
    Result<> Set(const std::string &key, const std::string &value) override;

    ~IniConfig() = default;
};
} // namespace fuse
