#pragma once

#include "entities/Device.hpp"
#include "DeviceInfoMapper.hpp"

class DeviceMapper {
public:
    enum Columns {
        ID = 0,
        VENDOR_ID,
        PRODUCT_ID,
        SERIAL,
        PRODUCT_NAME,
        VENDOR_NAME,
        VALID_TO
    };

    static Device fromRow(char** v) noexcept {
        DeviceInfoBuilder info;
        if (v[VENDOR_ID]) info.withVendorId(v[VENDOR_ID]);
        if (v[PRODUCT_ID]) info.withProductId(v[PRODUCT_ID]);
        if (v[SERIAL]) info.withSerial(v[SERIAL]);
        if (v[PRODUCT_NAME]) info.withProductName(v[PRODUCT_NAME]);
        if (v[VENDOR_NAME]) info.withVendorName(v[VENDOR_NAME]);

        DeviceBuilder device;
        if (v[VALID_TO]) device.withValidTo(v[VALID_TO]);

        return device
            .withId(std::stoull(v[ID]))
            .withInfo(info.build())
            .build();
    }
};