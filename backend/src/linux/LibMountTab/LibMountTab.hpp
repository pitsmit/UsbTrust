#pragma once

#include <expected>

#include <libmount/libmount.h>

#include "entities/MountMode.hpp"
#include "exceptions/Exceptions.hpp"

class LibMountTab {
    libmnt_table *tb = nullptr;

  public:
    explicit LibMountTab();

    ~LibMountTab() {
        mnt_free_table(tb);
    }

    std::expected<libmnt_fs *, LibMountError>
    findRecordFromMountPoint(std::string_view mountpoint) const;

    std::expected<libmnt_fs *, LibMountError> findRecordFromDevNode(std::string_view devNode) const;

    static std::expected<std::string, LibMountError> extractMountPoint(libmnt_fs *fs);

    static std::expected<const char *, LibMountError> getFSopts(libmnt_fs *fs);

    static std::expected<MountMode, LibMountError> extractMode(const char *opts);
};