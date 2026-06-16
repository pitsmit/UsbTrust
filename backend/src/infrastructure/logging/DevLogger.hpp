#pragma once

#include <chrono>
#include <fstream>
#include <format>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include "infrastructure/config/Config.hpp"

class Logger {
public:
    enum class Level {
        Debug = 0,
        Info,
        Warn,
        Error,
        Critical
    };
    
private:
    bool enabled = true;
    std::ofstream file;
    std::mutex mutex;
    Level currentLevel = Level::Info;

    static std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        auto tt = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    static const char* levelName(Level level) {
        switch (level) {
            case Level::Debug: return "DEBUG";
            case Level::Info: return "INFO";
            case Level::Warn: return "WARN";
            case Level::Error: return "ERROR";
            case Level::Critical: return "CRITICAL";
        }

        return "INFO";
    }

    void write(Level level, std::string_view message) {
        if (!enabled) return;
        if (static_cast<int>(level) < static_cast<int>(currentLevel))
            return;

        std::lock_guard lock(mutex);

        std::string line = std::format(
            "[{}] [{}] {}",
            timestamp(),
            levelName(level),
            message
        );

        std::cout << line << '\n';
        if (file.is_open()) {
            file << line << '\n';
            file.flush();
        }
    }

public:
    Logger() {
        file.open(Config::getLogFile(), std::ios::app);
        const auto lvl = Config::getLogLevel();

        if (lvl == "debug")
            currentLevel = Level::Debug;
        else if (lvl == "warn")
            currentLevel = Level::Warn;
        else if (lvl == "error")
            currentLevel = Level::Error;
        else if (lvl == "critical")
            currentLevel = Level::Critical;
        else
            currentLevel = Level::Info;
    }

    void setEnabled(bool value) {
        enabled = value;
    }

    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) {
        write(Level::Debug,
              std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        write(Level::Info,
              std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args) {
        write(Level::Warn,
              std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        write(Level::Error,
              std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void critical(std::format_string<Args...> fmt, Args&&... args) {
        write(Level::Critical,
              std::format(fmt, std::forward<Args>(args)...));
    }
};

class DevLogger {
private:
    std::shared_ptr<Logger> logger;
    DevLogger() : logger(std::make_shared<Logger>()) {}

public:
    static DevLogger& instance() noexcept {
        static DevLogger inst;
        return inst;
    }

    std::shared_ptr<Logger>& get() noexcept {
        return logger;
    }
};

#define mylog DevLogger::instance().get()