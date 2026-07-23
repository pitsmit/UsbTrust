#pragma once

#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"

class RecoveryService {
    MountRegistryManager &registrator;
    IUsbDeviceContextProvider &provider;
    MountCoordinator &coordinator;

  public:
    explicit RecoveryService(MountRegistryManager &registrator_,
                             IUsbDeviceContextProvider &provider_,
                             MountCoordinator &coordinator_)
        : registrator(registrator_), provider(provider_), coordinator(coordinator_) {}

    void run();
};