#pragma once

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"
#include "services/DeviceEventService/HandleStrategy/HandleStrategy.hpp"

class DeviceEventService {
    StrategyContext context;

  public:
    DeviceEventService(DeviceEventNotifyManager &notifier_,
                       IUsbDeviceContextProvider &provider_,
                       MountCoordinator &coordinator_)
        : context{notifier_, provider_, coordinator_} {}

    std::unique_ptr<HandleEventStrategy> create_strategy(EventType type);
    void handle(const DeviceEvent &event);
};