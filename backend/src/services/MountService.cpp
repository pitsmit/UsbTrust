#include "MountService.hpp"

#include <format>
#include <unordered_set>

#include "exceptions/Exceptions.hpp"
#include "entities/MountMode.hpp"

MountService::FsPermModel MountService::classifyPermModel(const std::string& fs) {
    static const std::unordered_set<std::string> nativePosix = {
        "ext2",
        "ext3",
        "ext4",
        "xfs",
        "btrfs"
    };
    return nativePosix.contains(fs)
        ? FsPermModel::NativePosix
        : FsPermModel::Emulated;
}

void MountService::mountDevice(
    std::string_view devnode,
    std::string_view mountPoint,
    MountMode mode) {

    const auto fs = sys.getFsType(devnode);
    const auto permModel = classifyPermModel(fs);
    const auto uid = 1000;
    const auto gid = 1000;
    std::string opts = "";

    if (permModel == FsPermModel::Emulated) {
        opts = std::format("{}, uid={},gid={},umask=0000", 
            mode.toStringLower(), uid, gid);
        if (fs.starts_with("ntfs")) {
            opts += ",windows_names";
        }
    }

    if (sys.mount(devnode, mountPoint, fs, mode, opts) < 0) {
        throw MountError(
            std::format("mount failed for devnode: {}, mountPoint: \
                {}, error: {}, opts: {}, fs: {}",
                devnode, mountPoint, strerror(errno), opts, fs)
        );
    }

    if (mode.isReadWrite() && permModel == FsPermModel::NativePosix) {
        sys.chown(mountPoint.data(), uid, gid);
        sys.chmod(mountPoint.data(), 0775);
    }
}

void MountService::handleUnmount(std::string_view mountPoint) {
    if (sys.umount(mountPoint) < 0) {
        throw UnMountError(
            std::format("unmount failed for mountPoint: {}, error: {}", 
                mountPoint, strerror(errno))
        );
    }
}

void MountService::remountDevice(
    std::string_view mountPoint, MountMode mode) {
    if (sys.remount(mountPoint, mode) < 0) {
        throw MountError(
            std::format("remount failed for mountPoint: {}, error: {}",
            mountPoint, strerror(errno)));
    }
}