#pragma once

#include "entities/Device.hpp"
#include "entities/DeviceInfo.hpp"
#include "entities/MountRecord.hpp"
#include "storage/mapping/Row.hpp"

class Mapper {
  public:
    template <typename T> static T from(const Row &r);
};

template <> Device Mapper::from<Device>(const Row &r);

template <> DeviceInfo Mapper::from<DeviceInfo>(const Row &r);

template <> MountRecord Mapper::from<MountRecord>(const Row &r);