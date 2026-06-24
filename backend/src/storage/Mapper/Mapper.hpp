#pragma once

#include "entities/Device.hpp"
#include "entities/DeviceInfo.hpp"
#include "entities/MountRecord.hpp"
#include "storage/Statement/Statement.hpp"

class Mapper {
  public:
    template <typename T> static T from(const Statement &r);
};

template <> inline DeviceInfo Mapper::from<DeviceInfo>(const Statement &r) {
    return DeviceInfoBuilder()
        .withVendorId(r.get<std::string>("vendor_id"))
        .withProductId(r.get<std::string>("product_id"))
        .withSerial(r.get<std::string>("serial"))
        .withVendorName(r.get<std::string>("vendor_name"))
        .withProductName(r.get<std::string>("product_name"))
        .build();
}

template <> inline Device Mapper::from<Device>(const Statement &r) {
    return DeviceBuilder()
        .withValidTo(r.get<std::string>("valid_to"))
        .withId(r.get<core::Id>("id"))
        .withInfo(Mapper::from<DeviceInfo>(r))
        .build();
}

template <> inline MountRecord Mapper::from<MountRecord>(const Statement &r) {
    return MountRecordBuilder()
        .withId(r.get<core::Id>("id"))
        .withDevNode(r.get<core::path>("dev_node"))
        .withMountPoint(r.get<core::path>("mount_point"))
        .withMode(MountMode::parse(r.get<std::string>("mode")))
        .withInfo(Mapper::from<DeviceInfo>(r))
        .build();
}