#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Exceptions.hpp"

class Config {
private:
    static inline constexpr std::string_view CONFIG_PATH = "config.txt";

    static const auto& getCache() {
        static const std::unordered_map<std::string, std::string> cfg = [] {
            std::unordered_map<std::string, std::string> m;
            std::ifstream file(CONFIG_PATH.data());
            if (!file)
                throw FileException((std::string(CONFIG_PATH) + " not found").c_str());
            std::string line;
            while (std::getline(file, line)) {
                auto pos = line.find('=');
                if (pos != std::string::npos)
                    m.emplace(line.substr(0, pos), line.substr(pos + 1));
            }
            return m;
        }();
        return cfg;
    }

    static std::string get(const std::string& key) {
        const auto& cache = getCache();
        auto it = cache.find(key);
        if (it == cache.end()) {
            throw FileException(("Missing config key: " + key).c_str());
        }
        return it->second;
    }

public:
    static std::vector<std::filesystem::path> getSchemaPaths() {
        std::vector<std::filesystem::path> result;
        for (const auto& entry :
            std::filesystem::directory_iterator(get("db.schema.dir"))) {
            result.push_back(entry.path());
        }
        return result;
    }
    static std::string getDBPath()  { return get("db.path"); }
    static std::string getLogFile() { return get("log.file"); }
    static std::string getLogLevel(){ return get("log.level"); }
    static int getHttpPort() { return std::stoi(get("http.port")); }
    static int getWebSocketPort() { return std::stoi(get("websocket.port")); }
};