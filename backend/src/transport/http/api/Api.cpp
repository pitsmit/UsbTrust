#include "Api.hpp"

#include "commands/DeviceCommands.hpp"
#include "transport/JsonUtils.hpp"

namespace {

constexpr const char *JsonMime = "application/json";

inline void sendJson(httplib::Response &res, const json &body, int status = 200) {
    res.status = status;
    res.set_content(body.dump(), JsonMime);
}

inline void sendStatus(httplib::Response &res, int status) {
    res.status = status;
}

inline void sendError(httplib::Response &res, const std::exception &e) {
    res.status = 500;
    res.set_content(e.what(), "text/plain");
}

template <typename Handler> auto safe(Handler &&handler) {
    return [handler = std::forward<Handler>(handler)](const httplib::Request &req,
                                                      httplib::Response &res) {
        try {
            handler(req, res);
        } catch (const std::exception &e) {
            sendError(res, e);
        }
    };
}

} // namespace

namespace api {

void DeviceApi::registerRoutes(httplib::Server &server) {
    server.Post("/api/v1/whitelist",
                safe([this](const auto &req, auto &res) { addWhitelist(req, res); }));
    server.Delete(R"(/api/v1/whitelist/(\d+))",
                  safe([this](const auto &req, auto &res) { deleteWhitelist(req, res); }));
    server.Patch(R"(/api/v1/whitelist/(\d+))",
                 safe([this](const auto &req, auto &res) { patchWhitelist(req, res); }));
    server.Get("/api/v1/whitelist",
               safe([this](const auto &req, auto &res) { getWhitelist(req, res); }));
    server.Get("/api/v1/list",
               safe([this](const auto &req, auto &res) { getConnectedDevices(req, res); }));

#ifdef ENABLE_TEST_API
    server.Post("/api/test/seed/whitelist",
                safe([this](const auto &req, auto &res) { seedWhitelist(req, res); }));
#endif
}

void DeviceApi::addWhitelist(const httplib::Request &req, httplib::Response &res) {
    auto body = json::parse(req.body);
    auto record = body.get<MountRecord>();

    AddDeviceToWhiteListCommand command(record);
    facade.execute(command);

    sendJson(res, {{"id", *command.record.device_id}}, 201);
}

void DeviceApi::deleteWhitelist(const httplib::Request &req, httplib::Response &res) {
    auto id = static_cast<size_t>(std::stoull(req.matches[1]));

    DeleteDeviceFromWhiteListCommand command(id);
    facade.execute(command);

    sendStatus(res, 204);
}

void DeviceApi::patchWhitelist(const httplib::Request &req, httplib::Response &res) {
    auto id = static_cast<size_t>(std::stoull(req.matches[1]));
    auto body = json::parse(req.body);
    std::optional<std::string> validTo = body["validTo"].get<std::string>();

    PatchValidToDeviceCommand command(id, validTo);
    facade.execute(command);

    sendStatus(res, 200);
}

void DeviceApi::getWhitelist(const httplib::Request &, httplib::Response &res) {
    GetWhiteListDeviceCommand command;
    facade.execute(command);

    sendJson(res, command.list);
}

void DeviceApi::getConnectedDevices(const httplib::Request &, httplib::Response &res) {
    GetCurrentConnectedDevicesCommand command;
    facade.execute(command);

    sendJson(res, command.records);
}

#ifdef ENABLE_TEST_API
void DeviceApi::seedWhitelist(const httplib::Request &req, httplib::Response &res) {
    sendStatus(res, 201);
}
#endif

} // namespace api