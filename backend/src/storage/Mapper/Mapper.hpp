#pragma once

#include "entities/Device.hpp"
#include "entities/DeviceInfo.hpp"
#include "entities/MountRecord.hpp"
#include "storage/Row/Row.hpp"

class Mapper {
  public:
    template <typename T> static T from(const Row &r);
};

template <> inline DeviceInfo Mapper::from<DeviceInfo>(const Row &r) {
    return DeviceInfoBuilder()
        .withVendorId(r.get<std::string>("vendor_id"))
        .withProductId(r.get<std::string>("product_id"))
        .withSerial(r.get<std::string>("serial"))
        .withVendorName(r.get<std::string>("vendor_name"))
        .withProductName(r.get<std::string>("product_name"))
        .build();
}

template <> inline Device Mapper::from<Device>(const Row &r) {
    return DeviceBuilder()
        .withValidTo(r.get<std::string>("valid_to"))
        .withId(r.get<std::uint64_t>("id"))
        .withInfo(Mapper::from<DeviceInfo>(r))
        .build();
}

template <> inline MountRecord Mapper::from<MountRecord>(const Row &r) {
    return MountRecordBuilder()
        .withId(r.get<std::uint64_t>("id"))
        .withDevNode(r.get<std::string>("dev_node"))
        .withMountPoint(r.get<std::string>("mount_point"))
        .withMode(MountMode::parse(r.get<std::string>("mode")))
        .withInfo(Mapper::from<DeviceInfo>(r))
        .build();
}