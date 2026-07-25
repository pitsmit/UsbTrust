#pragma once

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "managers/DeviceEventNotifyManager/DeviceEventNotifyManager.hpp"
#include "managers/MountCoordinator/MountCoordinator.hpp"
#include "ports/IUsbDeviceContextProvider.hpp"
#include "services/DeviceEventService/HandleStrategy/HandleStrategy.hpp"

class DeviceEventService {
    StrategyContext context;
    std::vector<std::unique_ptr<HandleEventStrategy>> strategies;

  public:
    DeviceEventService(DeviceEventNotifyManager &notifier_,
                       IUsbDeviceContextProvider &provider_,
                       MountCoordinator &coordinator_)
        : context{notifier_, provider_, coordinator_} {
        strategies.push_back(std::make_unique<HandleInsertEventStrategy>(context));
        strategies.push_back(std::make_unique<HandleRemoveEventStrategy>(context));
    }

    void handle(const DeviceEvent &event);
};