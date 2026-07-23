#pragma once

#include "entities/MountRecord/MountRecord.hpp"
#include "managers/MountManager/MountManager.hpp"
#include "managers/MountRegistryManager/MountRegistryManager.hpp"
#include "types/types.hpp"

class MountCoordinator {
    MountManager &mounter;
    MountRegistryManager &registrator;

  public:
    MountCoordinator(MountManager &mounter_, MountRegistryManager &registrator_)
        : mounter(mounter_), registrator(registrator_){};
    MountRecord mount(const core::path &node);
    void unmount(const MountRecord &record);
    void unmount(const core::path &mountPoint, const core::path &node);
    void remount(const MountRecord &record);
    void refresh(const MountRecord &record);
    void unmount(const core::path &mountPoint);
};