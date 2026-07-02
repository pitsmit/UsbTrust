#pragma once

#include <string>

class MountMode {
    enum Value { RO, RW };
    Value value_ = RO;

    explicit constexpr MountMode(Value value) : value_(value) {}

  public:
    MountMode() = default;
    static constexpr MountMode rw() {
        return MountMode(RW);
    }
    static constexpr MountMode ro() {
        return MountMode(RO);
    }

    static MountMode fromBool(bool is);
    bool isReadOnly() const noexcept;
    bool isReadWrite() const noexcept;
    bool operator==(const MountMode &mode) const noexcept;

    std::string toStringUpper() const noexcept;
    std::string toStringLower() const noexcept;
    static MountMode parse(std::string_view value);
};