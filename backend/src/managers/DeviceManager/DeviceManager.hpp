#pragma once

#include "repositories/DeviceRepository/DeviceRepository.hpp"

class DeviceManager {
  private:
    DeviceRepository repo;

  public:
    explicit DeviceManager(SqlExecutor &ex) : repo(ex) {}

    int addToWhitelist(const DeviceInfo &dev);
    std::vector<Device> getWhitelist();
    void removeFromWhitelist(core::Id id);
    void patchValidTo(core::Id id, std::optional<std::string> validTo);
    std::optional<core::Id> isAllowed(const DeviceInfo &dev);
};