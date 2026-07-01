#pragma once

#include "entities/MountRecord.hpp"
#include "managers/DeviceManager/DeviceManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"

class RecoveryService {
    MountRegistryManager &registrator;
    IUsbDeviceContextProvider &provider;
    DeviceManager &devman;
    MountCoordinator &coordinator;

    void actualize(MountRecord &rec);

  public:
    explicit RecoveryService(MountRegistryManager &registrator_,
                             IUsbDeviceContextProvider &provider_,
                             DeviceManager &devman_,
                             MountCoordinator &coordinator_)
        : registrator(registrator_), provider(provider_), devman(devman_),
          coordinator(coordinator_) {}

    void run();
};