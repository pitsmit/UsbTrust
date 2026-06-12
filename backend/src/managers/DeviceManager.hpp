#pragma once

#include "repositories/DeviceRepository.hpp"
#include "entities/DeviceInfo.hpp"
#include "services/types.hpp"

#include <vector>

class DeviceManager {
private:
    DeviceRepository repo;

public:
    explicit DeviceManager(DBConnection& db) : repo(db) {}

    int addToWhitelist(const DeviceInfo& dev) { return repo.add(dev); }

    std::vector<Device> getWhitelist() { return repo.getAll(); }

    void removeFromWhitelist(core::Id id) { repo.remove(id); }

    void patchValidTo(core::Id id, std::optional<std::string> validTo) {
        repo.updateValidTo(id, validTo);
    }

    std::optional<core::Id> isAllowed(const DeviceInfo& dev) { 
        return repo.findActiveId(dev); 
    }
};