#include "RecoveryService.hpp"

void RecoveryService::run() {
    for (const auto &record : registrator.getAll()) {
        coordinator.unmount(record.mountPoint);
    }
    registrator.clear();

    for (const auto &node : provider.getUsbDevNodes()) {
        coordinator.mount(node);
    }
}