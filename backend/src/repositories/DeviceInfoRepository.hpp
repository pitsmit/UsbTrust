#pragma once

#include "DBConnection.hpp"
#include "entities/DeviceInfo.hpp"
#include "RepositoryBase.hpp"
#include "types/types.hpp"

class DeviceInfoRepository : RepositoryBase {
public:
    explicit DeviceInfoRepository(
        DBConnection& connection)
        : RepositoryBase(connection) {}

    core::Id ensure(const DeviceInfo& info) {
        static constexpr auto sql =
            "INSERT INTO DeviceInfo "
            "(vendorId, productId, serial, productName, vendorName) "
            "VALUES (?, ?, ?, ?, ?) "
            "ON CONFLICT(vendorId, productId, serial) DO UPDATE SET "
            "productName = excluded.productName, "
            "vendorName = excluded.vendorName "
            "RETURNING id;";

        auto id = db.queryScalar<core::Id>(
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