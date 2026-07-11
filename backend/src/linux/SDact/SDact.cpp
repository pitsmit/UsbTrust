#include "SDact.hpp"

std::optional<DeviceEvent> SDact::toEvent() const {
    if (act == SD_DEVICE_ADD) {
        if (!dev.isUsbDevice())
            return std::nullopt;

        if (auto node = dev.getDevNode()) {
            return DeviceEvent{.type = EventType::INSERT, .devNode = *node};
        }
    }
    if (act == SD_DEVICE_REMOVE) {
        if (auto node = dev.getDevNode()) {
            return DeviceEvent{.type = EventType::REMOVE, .devNode = *node};
        }
    }
    return std::nullopt;
}