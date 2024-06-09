#include "fuse/configuration/configuration.hpp"

namespace fuse
{
namespace configuration
{

class Ini : public IConfiguration
{

    // this is the "global" space for key values
    std::shared_ptr<Section> m_root;

  public:
    Ini();
    Result<> Parse(const std::string &filepath);
    Result<> Load(std::istream &istream);

    Result<std::string> Get(const std::string &key) override;
    Result<Section> GetSection(const std::string &name) override;

    ~Ini() = default;
};

} // namespace configuration
} // namespace fuse
