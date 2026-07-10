#pragma once

#include <string>

#include "entities/MountMode/MountMode.hpp"
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

    int mountResult = 0;
    int umountResult = 0;

    void sync() noexcept override {
        syncCalled = true;
    }

    int mount(const core::path &dev,
              const core::path &target,
              std::string_view fs,
              const MountMode &,
              std::string_view opts) noexcept override {
        mountCalled = true;
        lastDev = dev;
        lastTarget = target;
        lastFs = fs;
        lastFlags = 1;
        lastOpts = opts;
        return mountResult;
    }

    int remount(const core::path &target, const MountMode &) noexcept override {
        mountCalled = true;
        lastTarget = target;
        lastFlags = 1;
        return mountResult;
    }

    int umount(const core::path &target) noexcept override {
        umountCalled = true;
        lastTarget = target;
        return umountResult;
    }

    void chown(const core::path &, int, int) noexcept override {}

    void chmod(const core::path &, int) noexcept override {}
};