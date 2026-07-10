#pragma once

#include "entities/Device/Device.hpp"
#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "entities/MountRecord/MountRecord.hpp"
#include "storage/Statement/Statement.hpp"

class Mapper {
  public:
    template <typename T> static T from(const Statement &r);
};

template <> inline DeviceInfo Mapper::from<DeviceInfo>(const Statement &r) {
    return DeviceInfo{.vendorId = r.get<std::string>("vendorId"),
                      .productId = r.get<std::string>("productId"),
                      .serial = r.get<std::string>("serial"),
                      .vendorName = r.get<std::string>("vendorName"),
                      .productName = r.get<std::string>("productName")};
}

template <> inline Device Mapper::from<Device>(const Statement &r) {
    return Device{.id = r.get<core::Id>("id"),
                  .info = Mapper::from<DeviceInfo>(r),
                  .validTo = r.get<std::string>("validTo")};
}

template <> inline MountRecord Mapper::from<MountRecord>(const Statement &r) {
    return MountRecord{.id = r.get<core::Id>("id"),
                       .devNode = r.get<core::path>("devNode"),
                       .mountPoint = r.get<core::path>("mountPoint"),
                       .info = Mapper::from<DeviceInfo>(r),
                       .mode = MountMode::parse(r.get<std::string>("mode"))};
}