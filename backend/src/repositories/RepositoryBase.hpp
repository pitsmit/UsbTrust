#pragma once

#include "DBConnection.hpp"
#include "DeviceInfo.hpp"

class RepositoryBase {
protected:
    DBConnection& db;

    explicit RepositoryBase(DBConnection& connection)
        : db(connection) {}

    static std::string sqlValue(
        const std::optional<std::string>& field)
    {
        return field
            ? "'" + *field + "'"
            : "NULL";
    }
};