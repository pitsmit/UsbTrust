#pragma once

#include "DBConnection.hpp"
#include "DeviceInfo.hpp"

class RepositoryBase {
protected:
    using Id = std::int64_t;
    
    DBConnection& db;

    explicit RepositoryBase(DBConnection& connection)
        : db(connection) {}
};