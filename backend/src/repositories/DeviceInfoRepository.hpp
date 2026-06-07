#pragma once

#include "DBConnection.hpp"
#include "DeviceInfo.hpp"
#include "RepositoryBase.hpp"

class DeviceInfoRepository : RepositoryBase {
public:
    explicit DeviceInfoRepository(
        DBConnection& connection)
        : RepositoryBase(connection) {}

    Id ensure(const DeviceInfo& info) {
        static constexpr auto sql =
            "SELECT id FROM DeviceInfo WHERE "
            "vendorId = ? AND "
            "productId = ? AND "
            "serial = ? LIMIT 1;";

        auto id = db.queryScalar<Id>(sql, info.vendorId, info.productId, info.serial);
        if (*id) return *id;

        static constexpr auto insert =
            "INSERT INTO DeviceInfo "
            "(vendorId, productId, serial, productName, vendorName) VALUES (?,?,?,?,?);";

        db.execute(insert,
            info.vendorId,
            info.productId,
            info.serial,
            info.productName,
            info.vendorName
        );
        return db.lastInsertId();
    }
};