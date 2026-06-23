#pragma once

#include <expected>

#include <libmount/libmount.h>

#include "entities/MountMode.hpp"
#include "exceptions/Exceptions.hpp"
#include "types/types.hpp"

class LibMountTab {
    libmnt_table *tb = nullptr;

  public:
    explicit LibMountTab();
    ~LibMountTab();

    std::expected<libmnt_fs *, LibMountError>
    findRecordFromMountPoint(const core::path &mountpoint) const;

    std::expected<libmnt_fs *, LibMountError>
    findRecordFromDevNode(const core::path &devNode) const;

    static std::expected<core::path, LibMountError> extractMountPoint(libmnt_fs *fs);

    static std::expected<const char *, LibMountError> getFSopts(libmnt_fs *fs);

    static std::expected<MountMode, LibMountError> extractMode(const char *opts);
};