#pragma once

#include <sys/mount.h>

#include "entities/MountMode.hpp"

class MountFlagsBuilder {
  private:
    static constexpr auto ZeroFlags() noexcept {
        return 0;
    }

    static constexpr auto ReadOnly() noexcept {
        return MS_RDONLY | MS_NOEXEC;
    }

  public:
    static auto from(MountMode md) noexcept {
        return md.isReadOnly() ? ReadOnly() : ZeroFlags();
    }

    static auto remount_from(MountMode md) noexcept {
        return from(md) | MS_REMOUNT;
    }
};