#include <string>
#include <unordered_map>

#include "types/types.hpp"

class ConfigFileBuilder {
    std::unordered_map<std::string, std::string> data;

  public:
    ConfigFileBuilder &add(const std::string &key, const std::string &value);
    void build(const core::path &path = "config.txt") const;
};