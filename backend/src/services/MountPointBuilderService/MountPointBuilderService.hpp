#pragma once

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "types/types.hpp"

class MountPointBuilderService {
  public:
    static core::path build(const DeviceInfo &dev);
    static void ensureExists(const core::path &path);
};