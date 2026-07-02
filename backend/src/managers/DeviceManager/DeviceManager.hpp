#pragma once

#include "repositories/DeviceRepository/DeviceRepository.hpp"

class DeviceManager {
    DeviceRepository repository;

  public:
    explicit DeviceManager(SqlExecutor &executor) : repository(executor) {}

    int addToWhitelist(const DeviceInfo &dev);
    std::vector<Device> getWhitelist();
    void removeFromWhitelist(core::Id id);
    void patchValidTo(core::Id id, std::optional<std::string> validTo);
    std::optional<core::Id> isAllowed(const DeviceInfo &dev);
};