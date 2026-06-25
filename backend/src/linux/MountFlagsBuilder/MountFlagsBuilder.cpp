#include "MountFlagsBuilder.hpp"

#include <sys/mount.h>

#include "entities/MountMode/MountMode.hpp"

constexpr int MountFlagsBuilder::ZeroFlags() noexcept {
    return 0;
}

constexpr int MountFlagsBuilder::ReadOnly() noexcept {
    return MS_RDONLY | MS_NOEXEC;
}

MountFlagsBuilder &MountFlagsBuilder::withFlagsFrom(const MountMode &md) noexcept {
    flags = md.isReadOnly() ? ReadOnly() : ZeroFlags();
    return *this;
}

MountFlagsBuilder &MountFlagsBuilder::withRemount() noexcept {
    flags |= MS_REMOUNT;
    return *this;
}

int MountFlagsBuilder::build() const noexcept {
    return flags;
}