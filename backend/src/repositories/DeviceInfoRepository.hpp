#pragma once

#include "RepositoryBase.hpp"
#include "types/types.hpp"

class DBConnection;
class DeviceInfo;

class DeviceInfoRepository : RepositoryBase {
public:
    explicit DeviceInfoRepository(
        DBConnection& connection)
        : RepositoryBase(connection) {}

    core::Id ensure(const DeviceInfo& info);
};