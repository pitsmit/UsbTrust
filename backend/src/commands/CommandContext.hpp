#pragma once

class DeviceManager;
class MountManager;
class MountRegistryManager;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistryManager& mountRegistry;
    MountManager& mountService;
};