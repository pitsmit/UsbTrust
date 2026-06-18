#pragma once

#include "entities/Device.hpp"
#include "repositories/Row.hpp"

class DeviceMapper {
  public:
    static Device fromRow(const Row& r) noexcept {
        DeviceInfoBuilder info;
        info.withVendorId(r.get<std::string>("vendor_id"));
        info.withProductId(r.get<std::string>("product_id"));
        info.withSerial(r.get<std::string>("serial"));
        info.withProductName(r.get<std::string>("product_name"));
        info.withVendorName(r.get<std::string>("vendor_name"));

        DeviceBuilder device;
        device.withValidTo(r.get<std::string>("valid_to"));

        return device
            .withId(r.get<std::uint64_t>("id"))
            .withInfo(info.build())
            .build();
    }
};