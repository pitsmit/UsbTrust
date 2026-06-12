#pragma once

#include <string>
#include <unordered_set>
#include <format>
#include <sys/stat.h>

#include "services/DevLogger.hpp"
#include "core/IMountSystem.hpp"
#include "exceptions/Exceptions.hpp"
#include "entities/MountMode.hpp"

class MountUtils {
private:
    IMountSystem &sys;

    enum class FsPermModel {
        NativePosix,
        Emulated
    };

    static FsPermModel classifyPermModel(const std::string& fs) {
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
public:
    explicit MountUtils(IMountSystem& s) : sys(s) {}

    void mountDevice(
        std::string_view devnode,
        std::string_view mountPoint,
        MountMode mode) {

        const auto fs = sys.getFsType(devnode);
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

        if (sys.mount(devnode, mountPoint, fs, mode.isReadOnly(), opts) < 0) {
            throw MountError(
                std::format("mount failed for devnode: {}, mountPoint: \
                    {}, error: {}, opts: {}, fs: {}",
                    devnode, mountPoint, strerror(errno), opts, fs)
            );
        }

        if (mode.isReadWrite() && permModel == FsPermModel::NativePosix) {
            chown(mountPoint.data(), uid, gid);
            chmod(mountPoint.data(), 0775);
        }

        mylog->info("Mounted: {}", mountPoint);
    }

    void handleUnmount(std::string_view mountPoint) {
        if (sys.umount(mountPoint) < 0) {
            throw UnMountError(
                std::format("unmount failed for mountPoint: {}, error: {}", 
                    mountPoint, strerror(errno))
            );
        }

        mylog->info("Unmounted: {}", mountPoint);
    }

    void remountDevice(std::string_view mountPoint, MountMode mode) {
        if (sys.remount(mountPoint, mode.isReadOnly()) < 0) {
            throw MountError(
                std::format("remount failed for mountPoint: {}, error: {}",
                mountPoint, strerror(errno)));
        }

        mylog->info("Remounted: {}", mountPoint);
    }
};