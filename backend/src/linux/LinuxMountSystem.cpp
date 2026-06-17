#include <sys/mount.h>
#include <sys/stat.h>
#include <libudev.h>
#include <unistd.h>
#include <blkid/blkid.h>

#include "LinuxMountSystem.hpp"
#include "MountFlagsBuilder.hpp"

int LinuxMountSystem::mount(
    std::string_view dev,
    std::string_view target,
    std::string_view fs,
    MountMode mode,
    std::string_view opts) noexcept
{
    return ::mount(
        dev.data(),
        target.data(),
        fs.data(),
        MountFlagsBuilder::from(mode),
        opts.empty() ? nullptr : opts.data()
    );
}

int LinuxMountSystem::remount(
    std::string_view target,
    MountMode mode) noexcept
{
    return ::mount(
        nullptr,
        target.data(),
        nullptr,
        MountFlagsBuilder::remount_from(mode),
        nullptr
    );
}

int LinuxMountSystem::umount(std::string_view target) noexcept {
    return ::umount2(target.data(), MNT_DETACH);
}

void LinuxMountSystem::sync() noexcept {
    ::sync();
}

void LinuxMountSystem::chown(std::string_view target, int uid, int gid) noexcept {
    ::chown(target.data(), uid, gid);
}

void LinuxMountSystem::chmod(std::string_view target, int perms) noexcept {
    ::chmod(target.data(), 0775);
}

std::string LinuxMountSystem::getFsType(std::string_view devnode) {
    blkid_cache cache = nullptr;
    if (blkid_get_cache(&cache, nullptr) != 0) {
        return "";
    }
    char* type = blkid_get_tag_value(cache, "TYPE", devnode.data());
    blkid_put_cache(cache);
    return type ? type : "";
}