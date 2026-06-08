#pragma once

#include <string>
#include <filesystem>

#include "DeviceInfo.hpp"

class MountPointBuilder {
public:
    static std::string build(const DeviceInfo& dev) {
        std::string base = "/media/dlp";
        return base + "/" + 
            dev.vendorId + "_" + 
            dev.productId + "_" + 
            dev.serial;
    }

    static void ensureExists(const std::string& path) {
        std::filesystem::create_directories(path);
    }
};