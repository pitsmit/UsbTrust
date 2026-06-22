#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Config {
  private:
    static inline constexpr std::string_view CONFIG_PATH = "config.txt";

    static const auto &getCache();
    static std::string get(std::string_view key);
    static int getNumber(std::string_view name);

  public:
    static std::vector<std::filesystem::path> getSchemaPaths();
    static std::string getDBPath();
    static std::string getLogFile();
    static std::string getLogLevel();
    static int getHttpPort();
    static int getWebSocketPort();
};