#pragma once

#include "DeviceInfoRepository.hpp"
#include "RepositoryBase.hpp"

class MountRecord;

class MountRepository : public RepositoryBase {
  private:
    DeviceInfoRepository info_rep;

  public:
    explicit MountRepository(DBConnection &connection)
        : RepositoryBase(connection), info_rep(connection) {}

    void add(const MountRecord &record);
    void update(const MountRecord &record);
    std::optional<std::string> getMountPointByDevNode(std::string_view devNode);
    std::optional<MountRecord> getById(core::Id id);
    void removeByDevNode(std::string_view devNode);
    std::vector<MountRecord> getAll();
};