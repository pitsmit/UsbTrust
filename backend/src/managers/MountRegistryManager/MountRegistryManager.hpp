#pragma once

#include "entities/MountRecord.hpp"
#include "repositories/MountRepository/MountRepository.hpp"

class MountRegistryManager {
  private:
    MountRepository repo;

  public:
    explicit MountRegistryManager(SqlExecutor &ex) : repo(ex) {}

    void add(const MountRecord &record);
    std::optional<core::path> getMountPointByDevNode(const core::path &devNode);
    std::optional<MountRecord> getById(core::Id id);
    void removeByDevNode(const core::path &devNode);
    void recreate(const MountRecord &record);
    std::vector<MountRecord> getAll();
    void refresh(const MountRecord &record);
};