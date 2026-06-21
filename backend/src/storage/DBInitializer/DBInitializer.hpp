#pragma once

#include <filesystem>
#include <string>

class SqlExecutor;

class DBInitializer {
  private:
    static std::string loadSQL(const std::filesystem::path &path);

  public:
    static void init(SqlExecutor &executor);
};