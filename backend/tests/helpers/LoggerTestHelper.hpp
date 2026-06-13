#pragma once

#include "services/DevLogger.hpp"

class LoggerTestHelper {
public:
    void disable() {
        DevLogger::instance().get()->setEnabled(false);
    }

    void restore() {
        DevLogger::instance().get()->setEnabled(true);
    }
};