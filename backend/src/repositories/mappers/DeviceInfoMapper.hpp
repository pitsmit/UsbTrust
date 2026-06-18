#pragma once

#include <sqlite3.h>

#include "entities/DeviceInfo.hpp"
#include "repositories/Row.hpp"

class DeviceInfoMapper {
public:
    static DeviceInfo fromRow(const Row& r) noexcept {
        DeviceInfoBuilder builder;
        builder.withVendorId(r.get<std::string>("vendor_id"));
        builder.withProductId(r.get<std::string>("product_id"));
        builder.withSerial(r.get<std::string>("serial"));
        builder.withVendorName(r.get<std::string>("vendor_name"));
        builder.withProductName(r.get<std::string>("product_name"));
        return builder.build();
    }
};