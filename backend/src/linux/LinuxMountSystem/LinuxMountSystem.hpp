#pragma once

#include "ports/IMountSystem.hpp"

class LinuxMountSystem : public IMountSystem {
  public:
    int mount(std::string_view dev,
              std::string_view target,
              std::string_view fs,
              MountMode mode,
              std::string_view opts) noexcept override;
    int remount(std::string_view target, MountMode mode) noexcept override;
    int umount(std::string_view target) noexcept override;
    void sync() noexcept override;
    void chown(std::string_view target, int uid, int gid) noexcept override;
    void chmod(std::string_view target, int perms) noexcept override;
    std::string getFsType(std::string_view dev) override;
};