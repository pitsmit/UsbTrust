#include "DeviceManager.hpp"

int DeviceManager::addToWhitelist(const DeviceInfo &dev) {
    return repository.add(dev);
}

std::vector<Device> DeviceManager::getWhitelist() {
    return repository.getAll();
}

void DeviceManager::removeFromWhitelist(core::Id id) {
    repository.remove(id);
}

void DeviceManager::patchValidTo(core::Id id, std::optional<std::string> validTo) {
    repository.updateValidTo(id, validTo);
}

std::optional<core::Id> DeviceManager::isAllowed(const DeviceInfo &dev) {
    return repository.findActiveId(dev);
}