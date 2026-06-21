#pragma once

#include "entities/MountRecord.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "managers/MountManager/MountManager.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"
#include "ports/IDeviceResolver.hpp"

class RecoveryService {
  private:
    MountRegistryManager &registry;
    IUsbDeviceContextProvider &resolver;
    MountManager &manager;
    DeviceManager &devman;

    void actualize(MountRecord &rec);

  public:
    explicit RecoveryService(MountRegistryManager &mr,
                             IUsbDeviceContextProvider &rs,
                             MountManager &man,
                             DeviceManager &devman_)
        : registry(mr), resolver(rs), manager(man), devman(devman_) {}

    void run();
};