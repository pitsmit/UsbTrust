#include "MountRegistryManager.hpp"

#include <optional>
#include <vector>

void MountRegistryManager::add(const MountRecord &record) {
    repository.add(record);
}

std::optional<MountRecord> MountRegistryManager::getById(core::Id id) {
    return repository.getByDevice(id);
}

void MountRegistryManager::removeByDevNode(const core::path &devNode) {
    repository.removeByDevNode(devNode);
}

std::vector<MountRecord> MountRegistryManager::getAll() {
    return repository.getAll();
}

void MountRegistryManager::clear() {
    repository.clear();
}

void MountRegistryManager::refresh(const MountRecord &record) {
    repository.update(record);
}