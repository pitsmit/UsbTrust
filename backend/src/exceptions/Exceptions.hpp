#pragma once

#include <algorithm>
#include <cstring>
#include <exception>
#include <string_view>

class BaseException : public std::exception {
  protected:
    static constexpr size_t sizebuff = 128;
    char errormsg[sizebuff]{};

  public:
    BaseException() noexcept = default;

    explicit BaseException(std::string_view msg) noexcept {
        const auto len = std::min(msg.size(), sizebuff - 1);
        std::memcpy(errormsg, msg.data(), len);
        errormsg[len] = '\0';
    }

    ~BaseException() override = default;

    const char *what() const noexcept override {
        return errormsg;
    }
};

class FileException : public BaseException {
  public:
    FileException() noexcept : BaseException("LoadError") {}

    explicit FileException(std::string_view msg) noexcept : BaseException(msg) {}
};

class HttpServerError : public BaseException {
  public:
    HttpServerError() noexcept : BaseException("HttpServerError") {}

    explicit HttpServerError(std::string_view msg) noexcept : BaseException(msg) {}
};

class SqlDataBaseError : public BaseException {
  public:
    SqlDataBaseError() noexcept : BaseException("SqlDataBaseError") {}

    explicit SqlDataBaseError(std::string_view msg) noexcept : BaseException(msg) {}
};

class ResolveInfoError : public BaseException {
  public:
    ResolveInfoError() noexcept : BaseException("ResolveInfoError") {}

    explicit ResolveInfoError(std::string_view msg) noexcept : BaseException(msg) {}
};

class UnknownFsError : public BaseException {
  public:
    UnknownFsError() noexcept : BaseException("UnknownFsError") {}

    explicit UnknownFsError(std::string_view msg) noexcept : BaseException(msg) {}
};

class MountError : public BaseException {
  public:
    MountError() noexcept : BaseException("MountError") {}

    explicit MountError(std::string_view msg) noexcept : BaseException(msg) {}
};

class UnMountError : public BaseException {
  public:
    UnMountError() noexcept : BaseException("UnMountError") {}

    explicit UnMountError(std::string_view msg) noexcept : BaseException(msg) {}
};