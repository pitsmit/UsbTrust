#pragma once

#include "repositories/MountRepository.hpp"
#include "entities/MountRecord.hpp"

class MountRegistryManager {
private:
    MountRepository repo;

public:
    explicit MountRegistryManager(DBConnection& db) : repo(db) {}

    void add(const MountRecord& record);
    std::optional<std::string> 
    getMountPointByDevNode(std::string_view devNode);
    std::optional<MountRecord> getById(core::Id id);
    void removeByDevNode(std::string_view devNode);
    void recreate(const MountRecord& record);
    std::vector<MountRecord> getAll();
    void refresh(const MountRecord& record);
};