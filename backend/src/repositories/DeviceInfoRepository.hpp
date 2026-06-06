#pragma once

#include "DBConnection.hpp"
#include "DeviceInfo.hpp"
#include "RepositoryBase.hpp"

class DeviceInfoRepository : RepositoryBase {
public:
    explicit DeviceInfoRepository(
        DBConnection& connection)
        : RepositoryBase(connection) {}

    int ensure(const DeviceInfo& info) {
        const std::string sql =
            "SELECT id FROM DeviceInfo WHERE "
            "vendorId = " + sqlValue(info.vendorId) + " AND "
            "productId = " + sqlValue(info.productId) + " AND "
            "serial = " + sqlValue(info.serial) + " LIMIT 1;";

        auto id = db.queryScalar<int>(sql);
        if (*id) return *id;

        const std::string insert =
            "INSERT INTO DeviceInfo "
            "(vendorId, productId, serial, productName, vendorName) VALUES (" +
            sqlValue(info.vendorId) + "," +
            sqlValue(info.productId) + "," +
            sqlValue(info.serial) + "," +
            sqlValue(info.productName) + "," +
            sqlValue(info.vendorName) + ");";

        db.execute(insert);
        return db.lastInsertId();
    }
};