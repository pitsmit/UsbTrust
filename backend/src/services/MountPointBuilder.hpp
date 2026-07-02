#pragma once

#include <filesystem>

#include "entities/DeviceInfo/DeviceInfo.hpp"
#include "types/types.hpp"

class MountPointBuilder {
  public:
    static auto build(const DeviceInfo &dev) {
        return core::path("/media/dlp") / dev.vendorId / dev.productId / dev.serial;
    }

    static void ensureExists(const core::path &path) {
        std::filesystem::create_directories(path);
    }
};