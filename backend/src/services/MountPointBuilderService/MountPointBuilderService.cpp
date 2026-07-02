#include "MountPointBuilderService.hpp"

#include <filesystem>

core::path MountPointBuilderService::build(const DeviceInfo &dev) {
    return core::path("/media/dlp") / dev.vendorId / dev.productId / dev.serial;
}

void MountPointBuilderService::ensureExists(const core::path &path) {
    std::filesystem::create_directories(path);
}