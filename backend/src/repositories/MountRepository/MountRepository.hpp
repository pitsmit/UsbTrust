#pragma once

#include "repositories/DeviceInfoRepository/DeviceInfoRepository.hpp"
#include "repositories/RepositoryBase.hpp"

struct MountRecord;
class SqlExecutor;

class MountRepository : public RepositoryBase {
  private:
    DeviceInfoRepository info_rep;

  public:
    explicit MountRepository(SqlExecutor &exec) : RepositoryBase(exec), info_rep(exec) {}

    void add(const MountRecord &record);
    void update(const MountRecord &record);
    std::optional<MountRecord> getByDevice(core::Id id);
    void removeByDevNode(const core::path &devNode);
    std::vector<MountRecord> getAll();
};