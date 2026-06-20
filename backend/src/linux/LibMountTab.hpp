#pragma once

#include <expected>
#include <format>

#include <libmount/libmount.h>

#include "entities/MountMode.hpp"
#include "exceptions/Exceptions.hpp"

class LibMountTab {
    libmnt_table *tb = nullptr;

  public:
    explicit LibMountTab() {
        tb = mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb) {
            throw LibMountError("Failed to create mount table");
        }
    }

    ~LibMountTab() {
        mnt_free_table(tb);
    }

    std::expected<libmnt_fs *, LibMountError>
    findRecordFromMountPoint(std::string_view mountpoint) const {
        auto fs = mnt_table_find_target(tb, mountpoint.data(), MNT_ITER_BACKWARD);
        if (!fs) {
            return std::unexpected<LibMountError>(
                std::format("Failed to find record for mountpoint: {}", mountpoint));
        }
        return fs;
    }

    std::expected<libmnt_fs *, LibMountError>
    findRecordFromDevNode(std::string_view devNode) const {
        auto fs = mnt_table_find_source(tb, devNode.data(), MNT_ITER_BACKWARD);
        if (!fs) {
            return std::unexpected<LibMountError>(
                std::format("Failed to find record for devNode: {}", devNode));
        }
        return fs;
    }

    static std::expected<std::string, LibMountError> extractMountPoint(libmnt_fs *fs) {
        auto target = mnt_fs_get_target(fs);
        if (!target) {
            return std::unexpected<LibMountError>("No mountpoint found");
        }
        std::string mountPoint(target);
        return mountPoint;
    }

    static std::expected<const char *, LibMountError> getFSopts(libmnt_fs *fs) {
        auto opts = mnt_fs_get_options(fs);
        if (!opts) {
            return std::unexpected<LibMountError>("Failed to extract filesystem options");
        }
        return opts;
    }

    static std::expected<MountMode, LibMountError> extractMode(const char *opts) {
        if (mnt_optstr_get_option(opts, "rw", nullptr, nullptr) == 0)
            return MountMode::rw();
        else if (mnt_optstr_get_option(opts, "ro", nullptr, nullptr) == 0)
            return MountMode::ro();
        return std::unexpected<LibMountError>("No mode found in options");
    }
};