#pragma once

#include "types/types.hpp"

enum EventType { INSERT, REMOVE };

struct DeviceEvent {
    EventType type;
    core::path devNode;
};

class DeviceEventBuilder {
    DeviceEvent event_;

  public:
    DeviceEventBuilder &withType(EventType type) noexcept;
    DeviceEventBuilder &withDevNode(const core::path &node) noexcept;
    DeviceEvent build() const;
};