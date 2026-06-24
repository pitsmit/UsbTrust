#include "DeviceInfoRepository.hpp"

#include "entities/DeviceInfo.hpp"
#include "types/types.hpp"

core::Id DeviceInfoRepository::ensure(const DeviceInfo &info) {
    static constexpr auto sql = "INSERT INTO DeviceInfo "
                                "(vendorId, productId, serial, productName, vendorName) "
                                "VALUES (?, ?, ?, ?, ?) "
                                "ON CONFLICT(vendorId, productId, serial) DO UPDATE SET "
                                "productName = excluded.productName, "
                                "vendorName = excluded.vendorName "
                                "RETURNING id;";

    auto id = executor.scalar<core::Id>(
        sql, info.vendorId, info.productId, info.serial, info.productName, info.vendorName);
    if (!id)
        throw id.error();
    return *id;
}