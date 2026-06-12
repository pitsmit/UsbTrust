#pragma once

#include <string>
#include <filesystem>

#include "DeviceInfo.hpp"

class MountPointBuilder {
public:
    static std::string build(const DeviceInfo& dev) {
        return std::format("/media/dlp/{}_{}_{}", 
            dev.vendorId, dev.productId, dev.serial);
    }

    static void ensureExists(std::string_view path) {
        std::filesystem::create_directories(path);
    }
};