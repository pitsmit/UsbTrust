#include "MountMode.hpp"

MountMode MountMode::fromBool(bool is) {
    return is ? rw() : ro();
}

bool MountMode::isReadOnly() const noexcept {
    return value_ == RO;
}

bool MountMode::isReadWrite() const noexcept {
    return value_ == RW;
}

bool MountMode::operator==(const MountMode &mode) const noexcept {
    return isReadWrite() == mode.isReadWrite();
}

std::string MountMode::toStringUpper() const noexcept {
    return value_ == RW ? "RW" : "RO";
}

std::string MountMode::toStringLower() const noexcept {
    return value_ == RW ? "rw" : "ro";
}

MountMode MountMode::parse(std::string_view value) {
    return value == "RW" ? rw() : ro();
}