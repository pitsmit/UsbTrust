#pragma once

#include <string>
#include <filesystem>

#include "DeviceInfo.hpp"

class MountPointBuilder {
public:
    static std::string build(const DeviceInfo& dev) {
        std::string base = "/media/dlp";
        std::string vid = *dev.vendorId;
        std::string pid = *dev.productId;
        std::string sn  = *dev.serial;
        return base + "/" + vid + "_" + pid + "_" + sn;
    }

    static void ensureExists(const std::string& path) {
        std::filesystem::create_directories(path);
    }
};