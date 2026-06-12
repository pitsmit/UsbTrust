#pragma once

#include <memory>
#include "DevLogger.hpp"

class LoggerTestHelper {
private:
    std::shared_ptr<Logger> oldLogger;

public:
    void disable() {
        oldLogger = DevLogger::instance().get();

        auto nullSink = std::make_shared<spdlog::sinks::null_sink_mt>();
        auto nullLogger = std::make_shared<spdlog::logger>("null", nullSink);

        DevLogger::instance().setLogger(nullLogger);
    }

    void restore() {
        DevLogger::instance().setLogger(oldLogger);
    }
};