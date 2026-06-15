#pragma once

#include <string>

class MountMode {
public:
    enum Value {
        RO,
        RW
    };

    explicit constexpr MountMode(Value value)
        : value_(value) {}

    constexpr MountMode() : value_(RO) {}

    static constexpr MountMode rw() {
        return MountMode(RW);
    }

    static constexpr MountMode ro() {
        return MountMode(RO);
    }

    template<typename T>
    static constexpr MountMode fromPresence(T obj) {
        return obj ? MountMode::rw()
                   : MountMode::ro();
    }

    bool isReadOnly() const noexcept {
        return value_ == RO;
    }

    bool isReadWrite() const noexcept {
        return value_ == RW;
    }

    bool operator==(const MountMode& mode) const noexcept {
        return isReadWrite() == mode.isReadWrite();
    }

    std::string toStringUpper() const noexcept {
        return value_ == RW ? "RW" : "RO";
    }

    std::string toStringLower() const noexcept {
        return value_ == RW ? "rw" : "ro";
    }

    static MountMode parse(std::string_view value) {
        return value == "RW"
            ? MountMode(RW)
            : MountMode(RO);
    }

private:
    Value value_;
};