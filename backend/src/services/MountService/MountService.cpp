#include "MountService.hpp"

#include <format>
#include <unordered_set>

#include "entities/MountMode/MountMode.hpp"
#include "exceptions/Exceptions.hpp"

MountService::FsPermModel MountService::classifyPermModel(const std::string &fs) {
    static const std::unordered_set<std::string> nativePosix = {
        "ext2", "ext3", "ext4", "xfs", "btrfs"};
    return nativePosix.contains(fs) ? FsPermModel::NativePosix : FsPermModel::Emulated;
}

void MountService::mount(const core::path &devnode, const core::path &mountPoint, MountMode &mode) {
    const auto fs = provider.getFsType(devnode);
    const auto permModel = classifyPermModel(fs);
    const auto uid = 1000;
    const auto gid = 1000;
    std::string opts = "";

    if (permModel == FsPermModel::Emulated) {
        opts = std::format("{}, uid={},gid={},umask=0000", mode.toStringLower(), uid, gid);
        if (fs.starts_with("ntfs")) {
            opts += ",windows_names";
        }
    }

    if (sys.mount(devnode, mountPoint, fs, mode, opts) < 0) {
        throw MountError(std::format("mount failed for devnode: {}, mountPoint: \
                {}, error: {}, opts: {}, fs: {}",
                                     devnode.c_str(),
                                     mountPoint.c_str(),
                                     strerror(errno),
                                     opts,
                                     fs));
    }

    if (mode.isReadWrite() && permModel == FsPermModel::NativePosix) {
        sys.chown(mountPoint, uid, gid);
        sys.chmod(mountPoint, 0775);
    }
}

void MountService::unmount(const core::path &mountPoint) {
    if (sys.umount(mountPoint) < 0) {
        throw UnMountError(std::format(
            "unmount failed for mountPoint: {}, error: {}", mountPoint.c_str(), strerror(errno)));
    }
}

void MountService::remount(const core::path &mountPoint, const MountMode &mode) {
    if (sys.remount(mountPoint, mode) < 0) {
        throw MountError(std::format(
            "remount failed for mountPoint: {}, error: {}", mountPoint.c_str(), strerror(errno)));
    }
}