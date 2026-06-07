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
            "INSERT INTO DeviceInfo "
            "(vendorId, productId, serial, productName, vendorName) "
            "VALUES (?, ?, ?, ?, ?) "
            "ON CONFLICT(vendorId, productId, serial) DO UPDATE SET "
            "productName = excluded.productName, "
            "vendorName = excluded.vendorName "
            "RETURNING id;";

        auto id = db.queryScalar<Id>(
            sql,
            info.vendorId,
            info.productId,
            info.serial,
            info.productName,
            info.vendorName
        );

        return *id;
    }
};