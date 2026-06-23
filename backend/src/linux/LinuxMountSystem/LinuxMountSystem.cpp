#include "LinuxMountSystem.hpp"

#include <blkid/blkid.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include "MountFlagsBuilder.hpp"

int LinuxMountSystem::mount(const core::path &dev,
                            const core::path &target,
                            std::string_view fs,
                            MountMode mode,
                            std::string_view opts) noexcept {
    return ::mount(dev.c_str(),
                   target.c_str(),
                   fs.data(),
                   MountFlagsBuilder().withFlagsFrom(mode).build(),
                   opts.empty() ? nullptr : opts.data());
}

int LinuxMountSystem::remount(const core::path &target, MountMode mode) noexcept {
    return ::mount(nullptr,
                   target.c_str(),
                   nullptr,
                   MountFlagsBuilder().withFlagsFrom(mode).withRemount().build(),
                   nullptr);
}

int LinuxMountSystem::umount(const core::path &target) noexcept {
    return ::umount2(target.c_str(), MNT_DETACH);
}

void LinuxMountSystem::sync() noexcept {
    ::sync();
}

void LinuxMountSystem::chown(const core::path &target, int uid, int gid) noexcept {
    ::chown(target.c_str(), uid, gid);
}

void LinuxMountSystem::chmod(const core::path &target, int perms) noexcept {
    ::chmod(target.c_str(), perms);
}