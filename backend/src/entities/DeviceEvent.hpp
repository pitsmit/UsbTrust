#pragma once

#include <string>

enum EventType {
    INSERT,
    REMOVE
};

struct DeviceEvent {
    EventType type;
    std::string devNode;
};


class DeviceEventBuilder {
private:
    DeviceEvent event_;

public:
    DeviceEventBuilder& withType(EventType type) noexcept {
        event_.type = type;
        return *this;
    }

    DeviceEventBuilder& withDevNode(std::string_view node) noexcept {
        event_.devNode = node;
        return *this;
    }

    DeviceEvent build() { return event_; }
};