#pragma once

#include <cstring>
#include <exception>
#include <string_view>

class BaseException : public std::exception {
  protected:
    static constexpr size_t sizebuff = 256;
    char errormsg[sizebuff]{};

  public:
    BaseException() noexcept = default;
    ~BaseException() override = default;

    explicit BaseException(std::string_view msg) noexcept;
    const char *what() const noexcept override;
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

class CreateMountPathError : public BaseException {
  public:
    CreateMountPathError() noexcept : BaseException("CreateMountPathError") {}

    explicit CreateMountPathError(std::string_view msg) noexcept : BaseException(msg) {}
};

class RecordNotFoundError : public BaseException {
  public:
    RecordNotFoundError() noexcept : BaseException("NoDataInDataBaseError") {}

    explicit RecordNotFoundError(std::string_view msg) noexcept : BaseException(msg) {}
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

class DeviceMonitorError : public BaseException {
  public:
    DeviceMonitorError() noexcept : BaseException("DeviceMonitorError") {}

    explicit DeviceMonitorError(std::string_view msg) noexcept : BaseException(msg) {}
};

class DeviceEnumeratorError : public BaseException {
  public:
    DeviceEnumeratorError() noexcept : BaseException("DeviceEnumeratorError") {}

    explicit DeviceEnumeratorError(std::string_view msg) noexcept : BaseException(msg) {}
};

class SDdeviceError : public BaseException {
  public:
    SDdeviceError() noexcept : BaseException("SDdeviceError") {}

    explicit SDdeviceError(std::string_view msg) noexcept : BaseException(msg) {}
};

class LibMountError : public BaseException {
  public:
    LibMountError() noexcept : BaseException("LibMountError") {}

    explicit LibMountError(std::string_view msg) noexcept : BaseException(msg) {}
};