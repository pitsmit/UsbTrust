#include "DeviceEventService.hpp"

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "services/DeviceEventService/HandleStrategy/HandleStrategy.hpp"

void DeviceEventService::handle(const DeviceEvent &event) {
    for (const auto &strategy : strategies) {
        if (strategy->can_handle(event.type)) {
            strategy->handle(event.devNode);
            return;
        }
    }
    throw UnSupportedEventError();
}