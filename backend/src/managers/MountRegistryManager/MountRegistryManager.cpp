#include "MountRegistryManager.hpp"

#include <optional>
#include <vector>

void MountRegistryManager::add(const MountRecord &record) {
    repo.add(record);
}

std::optional<MountRecord> MountRegistryManager::getById(core::Id id) {
    return repo.getById(id);
}

void MountRegistryManager::removeByDevNode(const core::path &devNode) {
    repo.removeByDevNode(devNode);
}

void MountRegistryManager::recreate(const MountRecord &record) {
    removeByDevNode(record.devNode);
    add(record);
}

std::vector<MountRecord> MountRegistryManager::getAll() {
    return repo.getAll();
}

void MountRegistryManager::refresh(const MountRecord &record) {
    repo.update(record);
}