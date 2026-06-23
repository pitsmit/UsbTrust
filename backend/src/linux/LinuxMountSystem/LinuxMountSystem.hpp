#pragma once

#include "ports/IMountSystem.hpp"
#include "types/types.hpp"

class LinuxMountSystem : public IMountSystem {
  public:
    int mount(const core::path &dev,
              const core::path &target,
              std::string_view fs,
              MountMode mode,
              std::string_view opts) noexcept override;
    int remount(const core::path &target, MountMode mode) noexcept override;
    int umount(const core::path &target) noexcept override;
    void sync() noexcept override;
    void chown(const core::path &target, int uid, int gid) noexcept override;
    void chmod(const core::path &target, int perms) noexcept override;
};