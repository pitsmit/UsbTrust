#pragma once

#include "entities/MountRecord.hpp"
#include "managers/MountRegistry.hpp"
#include "managers/MountManager.hpp"
#include "ports/IDeviceResolver.hpp"
#include "managers/DeviceManager.hpp"

class RecoveryService {
private:
    MountRegistryManager &registry;
    IDeviceResolver &resolver;
    MountManager &manager;
    DeviceManager &devman;

    void actualize(MountRecord &rec);

public:
    explicit RecoveryService(
        MountRegistryManager& mr, 
        IDeviceResolver &rs,
        MountManager& man,
        DeviceManager& devman_
    )
            : registry(mr),
              resolver(rs),
              manager(man),
              devman(devman_)
            {}

    void run();
};