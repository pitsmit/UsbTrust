#pragma once

#include "storage/executor/SqlExecutor.hpp"

class RepositoryBase {
  protected:
    SqlExecutor &executor;

    explicit RepositoryBase(SqlExecutor &exec) : executor(exec) {}
};