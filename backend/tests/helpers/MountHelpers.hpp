#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/mount.h>
#include <unistd.h>

#include "entities/MountMode/MountMode.hpp"

struct MountInfo {
    std::string mountPoint;
    MountMode mode;
};

struct LoopFs {
    std::string image;
    std::string device;
    std::string filesystem;
    std::string mountPoint;

    LoopFs() = default;
    LoopFs(const LoopFs &) = delete;
    LoopFs &operator=(const LoopFs &) = delete;

  public:
    void mnt() {
        if (device.empty())
            throw std::runtime_error("loop device is empty");
        if (!mountPoint.empty())
            throw std::runtime_error("already mounted");

        char templ[] = "/tmp/loopfs_XXXXXX";
        mountPoint = mkdtemp(templ);
        if (mountPoint.empty()) {
            throw std::runtime_error("mkdtemp failed");
        }

        if (mount(device.c_str(), mountPoint.c_str(), filesystem.c_str(), 0, nullptr) != 0) {
            std::filesystem::remove(mountPoint);
            mountPoint.clear();
            throw std::system_error(errno, std::generic_category(), "mount failed");
        }
    }

    MountInfo getMountInfo() const {
        if (device.empty())
            return {};

        std::ifstream mounts("/proc/self/mountinfo");
        if (!mounts)
            throw std::runtime_error("failed to open /proc/self/mountinfo");

        std::string line;
        while (std::getline(mounts, line)) {
            auto dash = line.find(" - ");
            if (dash == std::string::npos)
                continue;

            // Левая часть
            std::istringstream left(line.substr(0, dash));
            std::string id, parent, majorMinor, root, mnt, options;

            if (!(left >> id >> parent >> majorMinor >> root >> mnt >> options))
                continue;

            // Правая часть
            std::istringstream right(line.substr(dash + 3));
            std::string fsType, source, superOptions;

            if (!(right >> fsType >> source >> superOptions))
                continue;

            if (source != device)
                continue;

            MountInfo info;
            info.mountPoint = mnt;
            info.mode = options.starts_with("ro") || options.find(",ro") != std::string::npos
                            ? MountMode::ro()
                            : MountMode::rw();

            return info;
        }

        return {};
    }

    std::string currentMountPoint() const {
        return getMountInfo().mountPoint;
    }

    MountMode currentMountMode() const {
        return getMountInfo().mode;
    }

    bool isMounted() const {
        if (mountPoint.empty())
            return false;

        std::ifstream mounts("/proc/self/mountinfo");
        if (!mounts)
            throw std::runtime_error("failed to open /proc/self/mountinfo");

        std::string line;
        while (std::getline(mounts, line)) {
            std::istringstream iss(line);
            std::string id, parent, majorMinor, root, mnt;
            if (iss >> id >> parent >> majorMinor >> root >> mnt) {
                if (mnt == mountPoint)
                    return true;
            }
        }

        return false;
    }

    LoopFs(LoopFs &&other) noexcept
        : image(std::move(other.image)), device(std::move(other.device)) {
        other.image.clear();
        other.device.clear();
    }

    ~LoopFs() {
        if (!device.empty()) {
            std::string cmd = "losetup -d " + device + " >/dev/null 2>&1";
            std::system(cmd.c_str());
        }

        if (!image.empty()) {
            std::filesystem::remove(image);
        }
    }
};

inline LoopFs createLoopFs(const std::string &fsType) {
    LoopFs result;

    result.image =
        "/tmp/usb_" + std::to_string(::getpid()) + "_" + std::to_string(std::rand()) + ".img";

    result.filesystem = fsType;

    std::string mkfsCmd;
    if (fsType == "vfat") {
        mkfsCmd = "mkfs.vfat " + result.image;
    } else if (fsType == "ext4") {
        mkfsCmd = "mkfs.ext4 -F " + result.image;
    } else if (fsType == "ntfs") {
        mkfsCmd = "mkfs.ntfs -F " + result.image;
    } else if (fsType == "exfat") {
        mkfsCmd = "mkfs.exfat " + result.image;
    } else {
        throw std::runtime_error("unsupported filesystem: " + fsType);
    }

    int fd = ::open(result.image.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0)
        throw std::system_error(errno, std::generic_category());
    if (::ftruncate(fd, 32 * 1024 * 1024) != 0) {
        ::close(fd);
        throw std::system_error(errno, std::generic_category());
    }
    ::close(fd);

    if (std::system(mkfsCmd.c_str()) != 0) {
        throw std::runtime_error("failed to create filesystem");
    }

    FILE *pipe = popen(("losetup --find --show -P " + result.image).c_str(), "r");

    if (!pipe) {
        throw std::runtime_error("losetup failed");
    }

    char buffer[256]{};
    if (!fgets(buffer, sizeof(buffer), pipe)) {
        pclose(pipe);
        throw std::runtime_error("cannot obtain loop device");
    }
    pclose(pipe);
    result.device = buffer;

    while (!result.device.empty() &&
           (result.device.back() == '\n' || result.device.back() == '\r' ||
            result.device.back() == ' ')) {
        result.device.pop_back();
    }

    if (result.device.empty()) {
        throw std::runtime_error("empty loop device");
    }

    return result;
}