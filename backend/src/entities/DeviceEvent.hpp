#pragma once

#include "types/types.hpp"

enum EventType { INSERT, REMOVE };

struct DeviceEvent {
    EventType type;
    core::path devNode;
};

class DeviceEventBuilder {
  private:
    DeviceEvent event_;

  public:
    DeviceEventBuilder &withType(EventType type) noexcept {
        event_.type = type;
        return *this;
    }

    DeviceEventBuilder &withDevNode(const core::path &node) noexcept {
        event_.devNode = node;
        return *this;
    }

    DeviceEvent build() const {
        return event_;
    }
};