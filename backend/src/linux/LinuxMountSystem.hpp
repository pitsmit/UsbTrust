#pragma once
#include "core/IMountSystem.hpp"
#include "linux/MountFlagsBuilder.hpp"

#include <sys/mount.h>
#include <libudev.h>
#include <unistd.h>
#include <blkid/blkid.h>
#include <string>

class LinuxMountSystem : public IMountSystem {
public:
    int mount(
        std::string_view dev,
        std::string_view target,
        std::string_view fs,
        MountMode mode,
        std::string_view opts) noexcept override
    {
        return ::mount(
            dev.data(),
            target.data(),
            fs.data(),
            MountFlagsBuilder::from(mode),
            opts.empty() ? nullptr : opts.data()
        );
    }

    int remount(
        std::string_view target,
        MountMode mode) noexcept override
    {
        return ::mount(
            nullptr,
            target.data(),
            nullptr,
            MountFlagsBuilder::remount_from(mode),
            nullptr
        );
    }

    int umount(std::string_view target) noexcept override {
        return ::umount2(target.data(), MNT_DETACH);
    }

    void sync() noexcept override {
        ::sync();
    }

    std::string getFsType(std::string_view devnode) override {
        blkid_cache cache = nullptr;
        if (blkid_get_cache(&cache, nullptr) != 0) {
            return "";
        }
        char* type = blkid_get_tag_value(cache, "TYPE", devnode.data());
        blkid_put_cache(cache);
        return type ? type : "";
    }
};