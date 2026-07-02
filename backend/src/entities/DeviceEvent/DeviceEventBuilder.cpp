#include "DeviceEvent.hpp"

DeviceEventBuilder &DeviceEventBuilder::withType(EventType type) noexcept {
    event_.type = type;
    return *this;
}

DeviceEventBuilder &DeviceEventBuilder::withDevNode(const core::path &node) noexcept {
    event_.devNode = node;
    return *this;
}

DeviceEvent DeviceEventBuilder::build() const {
    return event_;
}