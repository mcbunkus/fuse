#pragma once

#include "fuse/result.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace fuse
{

namespace configuration
{

class Section
{
    std::string m_name;
    std::unordered_map<std::string, std::string> m_keyvals;
    std::unordered_map<std::string, std::shared_ptr<Section>> m_subSections;

  public:
    Section(const std::string &name);

    Section(const Section &) = default;
    Section(Section &&) = default;

    std::string Name() const;

    bool Has(const std::string &key);
    bool HasSection(const std::string &name);

    void Set(const std::string &key, const std::string &value);
    void Add(std::shared_ptr<Section> section);

    Result<std::string> Get(const std::string &key);
    Result<Section> GetSection(const std::string &name);
};

class IConfiguration
{
  public:
    virtual Result<std::string> Get(const std::string &key) = 0;
    virtual Result<Section> GetSection(const std::string &name) = 0;
    virtual ~IConfiguration() = default;
};

class IConfigurable
{
  public:
    virtual Result<> configure(const IConfiguration &) = 0;
    virtual ~IConfigurable() = default;
};

} // namespace configuration
} // namespace fuse
