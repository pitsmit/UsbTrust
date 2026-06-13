#pragma once

#include <string>

#include "core/IMountSystem.hpp"

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
        bool readOnly,
        std::string_view opts) noexcept override
    {
        mountCalled = true;
        lastDev = dev;
        lastTarget = target;
        lastFs = fs;
        lastFlags = readOnly;
        lastOpts = opts;
        return mountResult;
    }

    int remount(
        std::string_view target,
        bool readOnly) noexcept override
    {
        mountCalled = true;
        lastTarget = target;
        lastFlags = readOnly;
        return mountResult;
    }

    int umount(std::string_view target) noexcept override {
        umountCalled = true;
        lastTarget = target;
        return umountResult;
    }
};