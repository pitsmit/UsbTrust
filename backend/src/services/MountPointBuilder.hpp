#pragma once

#include <filesystem>

#include "entities/DeviceInfo.hpp"

class MountPointBuilder {
  public:
    static auto build(const DeviceInfo &dev) {
        return std::filesystem::path("/media/dlp") / dev.vendorId / dev.productId / dev.serial;
    }

    static void ensureExists(const std::filesystem::path &path) {
        std::filesystem::create_directories(path);
    }
};