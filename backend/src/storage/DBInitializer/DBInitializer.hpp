#pragma once

#include <string>

#include "types/types.hpp"

class SqlExecutor;

class DBInitializer {
  private:
    static std::string loadSQL(const core::path &path);

  public:
    static void init(SqlExecutor &executor);
};