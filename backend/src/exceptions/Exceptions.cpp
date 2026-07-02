#include "Exceptions.hpp"

BaseException::BaseException(std::string_view msg) noexcept {
    const auto len = std::min(msg.size(), sizebuff - 1);
    std::memcpy(errormsg, msg.data(), len);
    errormsg[len] = '\0';
}

const char *BaseException::what() const noexcept {
    return errormsg;
}