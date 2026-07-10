#include "ConfigFileBuilder.hpp"

#include <fstream>

ConfigFileBuilder &ConfigFileBuilder::add(const std::string &key, const std::string &value) {
    data[key] = value;
    return *this;
}

void ConfigFileBuilder::build(const core::path &path) const {
    std::ofstream f(path, std::ios::trunc);

    for (const auto &[k, v] : data) {
        f << k << "=" << v << "\n";
    }
}