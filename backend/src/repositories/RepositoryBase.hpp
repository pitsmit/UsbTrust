#pragma once

#include "storage/SqlExecutor/SqlExecutor.hpp"

class RepositoryBase {
  protected:
    SqlExecutor &executor;

    explicit RepositoryBase(SqlExecutor &exec) : executor(exec) {}
};