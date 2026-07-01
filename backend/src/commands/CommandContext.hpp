#pragma once

class DeviceManager;
class MountRegistryManager;
class MountCoordinator;

struct CommandContext {
    DeviceManager &deviceManager;
    MountRegistryManager &mountRegistry;
    MountCoordinator &coordinator;
};