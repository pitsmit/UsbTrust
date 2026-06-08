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

    bool isReadOnly() const {
        return value_ == RO;
    }

    bool isReadWrite() const {
        return value_ == RW;
    }

    std::string toString() const {
        return value_ == RW ? "RW" : "RO";
    }

    static MountMode parse(std::string_view value) {
        return value == "RW"
            ? MountMode(RW)
            : MountMode(RO);
    }

private:
    Value value_;
};