#pragma once

#include "repositories/DeviceInfoRepository/DeviceInfoRepository.hpp"
#include "repositories/RepositoryBase.hpp"

class MountRecord;
class SqlExecutor;

class MountRepository : public RepositoryBase {
  private:
    DeviceInfoRepository info_rep;

  public:
    explicit MountRepository(SqlExecutor &exec) : RepositoryBase(exec), info_rep(exec) {}

    void add(const MountRecord &record);
    void update(const MountRecord &record);
    std::optional<std::string> getMountPointByDevNode(std::string_view devNode);
    std::optional<MountRecord> getById(core::Id id);
    void removeByDevNode(std::string_view devNode);
    std::vector<MountRecord> getAll();
};