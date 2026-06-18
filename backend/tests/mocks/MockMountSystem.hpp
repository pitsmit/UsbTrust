#pragma once

#include <string>

#include "ports/IMountSystem.hpp"

class MockMountSystem : public IMountSystem {
public:
    bool syncCalled = false;

    bool mountCalled = false;
    bool umountCalled = false;

    std::string lastDev;
    std::string lastTarget;
    std::string lastFs;
    int lastFlags;
    std::string lastOpts;

    std::string fsType = "ext4";
    int mountResult = 0;
    int umountResult = 0;

    void sync() noexcept override {
        syncCalled = true;
    }

    std::string getFsType(std::string_view dev) override {
        lastDev = dev;
        return fsType;
    }

    int mount(
        std::string_view dev,
        std::string_view target,
        std::string_view fs,
        MountMode mode,
        std::string_view opts) noexcept override
    {
        mountCalled = true;
        lastDev = dev;
        lastTarget = target;
        lastFs = fs;
        lastFlags = 1;
        lastOpts = opts;
        return mountResult;
    }

    int remount(
        std::string_view target,
        MountMode mode) noexcept override
    {
        mountCalled = true;
        lastTarget = target;
        lastFlags = 1;
        return mountResult;
    }

    int umount(std::string_view target) noexcept override {
        umountCalled = true;
        lastTarget = target;
        return umountResult;
    }

    void chown(std::string_view target, int uid, int gid) noexcept override {

    }

    void chmod(std::string_view target, int perms) noexcept override {

    }
};