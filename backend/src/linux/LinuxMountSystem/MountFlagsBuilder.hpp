#pragma once

#include <sys/mount.h>

#include "entities/MountMode.hpp"

class MountFlagsBuilder {
  private:
    int flags;

    static constexpr auto ZeroFlags() noexcept {
        return 0;
    }

    static constexpr auto ReadOnly() noexcept {
        return MS_RDONLY | MS_NOEXEC;
    }

  public:
    auto withFlagsFrom(MountMode md) noexcept {
        flags = md.isReadOnly() ? ReadOnly() : ZeroFlags();
        return *this;
    }

    auto withRemount() noexcept {
        flags |= MS_REMOUNT;
        return *this;
    }

    int build() const noexcept {
        return flags;
    }
};