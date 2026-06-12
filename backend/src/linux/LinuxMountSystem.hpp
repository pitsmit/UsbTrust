#include "IMountSystem.hpp"

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
        bool readOnly,
        std::string_view opts) noexcept override
    {
        return ::mount(
            dev.data(),
            target.data(),
            fs.data(),
            readOnly ? (MS_RDONLY | MS_NOEXEC) : 0,
            opts.empty() ? nullptr : opts.data()
        );
    }

    int remount(
        std::string_view target,
        bool readOnly) noexcept override
    {
        return ::mount(
            nullptr,
            target.data(),
            nullptr,
            readOnly ? (MS_REMOUNT | MS_RDONLY | MS_NOEXEC) : MS_REMOUNT,
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