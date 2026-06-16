#pragma once

#include <string_view>

class MountMode;

class IMountSystem {
public:
    virtual int mount(
        std::string_view dev,
        std::string_view target,
        std::string_view fs,
        MountMode mode,
        std::string_view opts) noexcept = 0;
    virtual int remount(
        std::string_view target,
        MountMode mode) noexcept = 0;
    virtual int umount(std::string_view target) noexcept = 0;
    virtual void sync() noexcept = 0;
    virtual void chown(std::string_view target, int uid, int gid) noexcept = 0;
    virtual void chmod(std::string_view target, int perms) noexcept = 0;
    virtual std::string getFsType(std::string_view dev) = 0;
    virtual ~IMountSystem() = default;
};