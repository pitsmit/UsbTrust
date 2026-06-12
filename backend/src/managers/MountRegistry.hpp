#pragma once

#include <vector>
#include <string>
#include <optional>

#include "repositories/MountRepository.hpp"
#include "entities/MountRecord.hpp"

class MountRegistryManager {
private:
    MountRepository repo;

public:
    explicit MountRegistryManager(DBConnection& db) : repo(db) {}

    void add(const MountRecord& record) { repo.add(record); }

    std::optional<std::string> getMountPointByDevNode(std::string_view devNode) {
        return repo.getMountPointByDevNode(devNode);
    }

    std::optional<MountRecord> getById(core::Id id) { return repo.getById(id); }

    void removeByDevNode(std::string_view devNode) { repo.removeByDevNode(devNode); }

    void recreate(const MountRecord& record) {
        removeByDevNode(record.devNode);
        add(record);
    }

    std::vector<MountRecord> getAll() { return repo.getAll(); }
    void refresh(const MountRecord& record) { repo.update(record); }
};