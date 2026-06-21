#pragma once

#include "entities/Device.hpp"
#include "repositories/DeviceInfoRepository/DeviceInfoRepository.hpp"
#include "repositories/RepositoryBase.hpp"

class SqlExecutor;

class DeviceRepository : public RepositoryBase {
  private:
    DeviceInfoRepository info_rep;

  public:
    explicit DeviceRepository(SqlExecutor &exec) : RepositoryBase(exec), info_rep(exec) {}

    core::Id add(const DeviceInfo &dev);
    std::vector<Device> getAll();
    void remove(core::Id id);
    void updateValidTo(core::Id id, std::optional<std::string> validTo);
    std::optional<core::Id> findActiveId(const DeviceInfo &info);
};