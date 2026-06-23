#pragma once

#include <string_view>

#include "types/types.hpp"

class MountMode;

class IMountSystem {
  public:
    virtual int mount(const core::path &dev,
                      const core::path &target,
                      std::string_view fs,
                      MountMode mode,
                      std::string_view opts) noexcept = 0;
    virtual int remount(const core::path &target, MountMode mode) noexcept = 0;
    virtual int umount(const core::path &target) noexcept = 0;
    virtual void sync() noexcept = 0;
    virtual void chown(const core::path &target, int uid, int gid) noexcept = 0;
    virtual void chmod(const core::path &target, int perms) noexcept = 0;
    virtual std::string getFsType(const core::path &dev) = 0;
    virtual ~IMountSystem() = default;
};