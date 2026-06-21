#pragma once

#include "repositories/RepositoryBase.hpp"
#include "types/types.hpp"

class DeviceInfo;
class SqlExecutor;

class DeviceInfoRepository : RepositoryBase {
  public:
    explicit DeviceInfoRepository(SqlExecutor &exec) : RepositoryBase(exec) {}

    core::Id ensure(const DeviceInfo &info);
};