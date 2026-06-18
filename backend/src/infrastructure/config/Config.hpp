#pragma once

#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exceptions/Exceptions.hpp"

class Config {
  private:
    static inline constexpr std::string_view CONFIG_PATH = "config.txt";

    static const auto &getCache() {
        using config_map = std::unordered_map<std::string, std::string>;
        static const config_map cfg = [] {
            std::ifstream file(CONFIG_PATH.data());
            if (!file)
                throw FileException(std::format("{} not found", CONFIG_PATH));
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

    static std::string get(std::string_view key) {
        const auto &cache = getCache();
        auto it = cache.find(std::string(key));
        if (it == cache.end()) {
            throw FileException(std::format("Missing config key: {}", key));
        }
        return it->second;
    }

  public:
    static std::vector<std::filesystem::path> getSchemaPaths() {
        std::vector<std::filesystem::path> result;
        for (const auto &e : std::filesystem::directory_iterator(get("db.schema.dir")))
            result.push_back(e.path());
        return result;
    }
    static std::string getDBPath() {
        return get("db.path");
    }
    static std::string getLogFile() {
        return get("log.file");
    }
    static std::string getLogLevel() {
        return get("log.level");
    }
    static int getHttpPort() {
        return std::stoi(get("http.port"));
    }
    static int getWebSocketPort() {
        return std::stoi(get("websocket.port"));
    }
};