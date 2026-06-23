#include "Config.hpp"

#include <format>
#include <fstream>
#include <unordered_map>

#include "exceptions/Exceptions.hpp"

const auto &Config::getCache() {
    using config_map = std::unordered_map<std::string, std::string>;
    static const config_map cfg = [] {
        std::ifstream file(CONFIG_PATH);
        if (!file)
            throw FileException(std::format("{} not found!", CONFIG_PATH.c_str()));
        std::string line;
        config_map m;
        while (std::getline(file, line)) {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                m.emplace(line.substr(0, pos), line.substr(pos + 1));
        }
        return m;
    }();
    return cfg;
}

std::string Config::get(std::string_view key) {
    const auto &cache = getCache();
    auto it = cache.find(std::string(key));
    if (it == cache.end()) {
        throw FileException(std::format("Missing config key: {}!", key));
    }
    return it->second;
}

int Config::getNumber(std::string_view name) {
    try {
        return std::stoi(get(name));
    } catch (const std::invalid_argument &) {
        throw FileException(std::format("{} in config is not a number!", name));
    } catch (const std::out_of_range &) {
        throw FileException(std::format("{} in config is out of range!", name));
    }
}

std::vector<core::path> Config::getSchemaPaths() {
    try {
        std::vector<core::path> result;
        auto iter = std::filesystem::directory_iterator(get("db.schema.dir"));
        std::transform(begin(iter), end(iter), std::back_inserter(result), [](const auto &entry) {
            return entry.path();
        });
        std::ranges::sort(result);
        return result;
    } catch (const std::filesystem::filesystem_error &e) {
        throw FileException(e.what());
    }
}

core::path Config::getDBPath() {
    return get("db.path");
}

core::path Config::getLogFile() {
    return get("log.file");
}

std::string Config::getLogLevel() {
    return get("log.level");
}

int Config::getHttpPort() {
    return getNumber("http.port");
}

int Config::getWebSocketPort() {
    return getNumber("websocket.port");
}