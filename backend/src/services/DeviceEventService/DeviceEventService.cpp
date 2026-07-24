#include "DeviceEventService.hpp"

#include "entities/DeviceEvent/DeviceEvent.hpp"
#include "services/DeviceEventService/HandleStrategy/HandleStrategy.hpp"

std::unique_ptr<HandleEventStrategy> DeviceEventService::create_strategy(EventType type) {
    switch (type) {
    case EventType::INSERT:
        return std::make_unique<HandleInsertEventStrategy>(context);

    case EventType::REMOVE:
        return std::make_unique<HandleRemoveEventStrategy>(context);
    }

    throw std::logic_error("Unknown event type");
}

void DeviceEventService::handle(const DeviceEvent &event) {
    auto strategy = create_strategy(event.type);
    strategy->handle(event.devNode);
}