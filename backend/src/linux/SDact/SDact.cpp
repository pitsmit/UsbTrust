#include "SDact.hpp"

bool SDact::isValid() const {
    return act != _SD_DEVICE_ACTION_INVALID;
}

std::optional<DeviceEvent> SDact::toEvent(const SDdevView &dev) const {
    if (act == SD_DEVICE_ADD) {
        if (!dev.isUsbDevice())
            return std::nullopt;

        if (auto node = dev.getDevNode()) {
            return DeviceEventBuilder().withType(EventType::INSERT).withDevNode(*node).build();
        }
    }
    if (act == SD_DEVICE_REMOVE) {
        if (auto node = dev.getDevNode()) {
            return DeviceEventBuilder().withType(EventType::REMOVE).withDevNode(*node).build();
        }
    }
    return std::nullopt;
}