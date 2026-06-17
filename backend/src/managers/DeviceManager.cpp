#include "DeviceManager.hpp"

int DeviceManager::addToWhitelist(const DeviceInfo& dev) { 
    return repo.add(dev); 
}

std::vector<Device> DeviceManager::getWhitelist() { 
    return repo.getAll(); 
}

void DeviceManager::removeFromWhitelist(core::Id id) { 
    repo.remove(id); 
}

void DeviceManager::patchValidTo(
    core::Id id, std::optional<std::string> validTo) {
    repo.updateValidTo(id, validTo);
}

std::optional<core::Id> 
DeviceManager::isAllowed(const DeviceInfo& dev) { 
    return repo.findActiveId(dev); 
}