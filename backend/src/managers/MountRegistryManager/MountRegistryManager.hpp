#pragma once

#include "entities/MountRecord/MountRecord.hpp"
#include "repositories/MountRepository/MountRepository.hpp"

class MountRegistryManager {
    MountRepository repository;

  public:
    explicit MountRegistryManager(SqlExecutor &executor) : repository(executor) {}

    void add(const MountRecord &record);
    std::optional<MountRecord> getById(core::Id id);
    void removeByDevNode(const core::path &devNode);
    std::vector<MountRecord> getAll();
    void refresh(const MountRecord &record);
};