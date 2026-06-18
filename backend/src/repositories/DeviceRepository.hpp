#pragma once

#include "DeviceInfoRepository.hpp"
#include "RepositoryBase.hpp"
#include "entities/Device.hpp"

class DeviceRepository : public RepositoryBase {
  private:
    DeviceInfoRepository info_rep;

  public:
    explicit DeviceRepository(DBConnection &connection)
        : RepositoryBase(connection), info_rep(connection) {}

    core::Id add(const DeviceInfo &dev);
    std::vector<Device> getAll();
    void remove(core::Id id);
    void updateValidTo(core::Id id, std::optional<std::string> validTo);
    std::optional<core::Id> findActiveId(const DeviceInfo &info);
};