#pragma once

#include <string>
#include <vector>

#include "types/types.hpp"

class Config {
  private:
    static inline const core::path CONFIG_PATH = "config.txt";

    static const auto &getCache();
    static std::string get(std::string_view key);
    static int getNumber(std::string_view name);

  public:
    static std::vector<core::path> getSchemaPaths();
    static core::path getDBPath();
    static core::path getLogFile();
    static std::string getLogLevel();
    static int getHttpPort();
    static int getWebSocketPort();
};