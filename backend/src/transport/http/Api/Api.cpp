#include "Api.hpp"

#include "commands/DeviceCommands.hpp"

void Api::sendJson(response res, const json &body, int status) {
    res.status = status;
    res.set_content(body.dump(), "application/json");
}

void Api::sendStatus(response res, int status) {
    res.status = status;
}

void Api::sendError(response res, const std::exception &e) {
    res.status = 500;
    res.set_content(e.what(), "text/plain");
}

void Api::registerRoutes(httplib::Server &server) {
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

void Api::addWhitelist(request req, const response &res) {
    auto body = json::parse(req.body);
    auto record = body.get<MountRecord>();

    AddDeviceToWhiteListCommand command(record);
    facade.execute(command);

    sendJson(res, {{"id", *command.record.device_id}}, 201);
}

void Api::deleteWhitelist(request req, const response &res) {
    auto id = static_cast<size_t>(std::stoull(req.matches[1]));

    DeleteDeviceFromWhiteListCommand command(id);
    facade.execute(command);

    sendStatus(res, 204);
}

void Api::patchWhitelist(request req, const response &res) {
    auto id = static_cast<size_t>(std::stoull(req.matches[1]));
    auto body = json::parse(req.body);
    std::optional<std::string> validTo = body["validTo"].get<std::string>();

    PatchValidToDeviceCommand command(id, validTo);
    facade.execute(command);

    sendStatus(res, 200);
}

void Api::getWhitelist(request, const response &res) {
    GetWhiteListDeviceCommand command;
    facade.execute(command);

    sendJson(res, command.list);
}

void Api::getConnectedDevices(request, const response &res) {
    GetCurrentConnectedDevicesCommand command;
    facade.execute(command);

    sendJson(res, command.records);
}

#ifdef ENABLE_TEST_API
void DeviceApi::seedWhitelist(request req, const response &res) {
    sendStatus(res, 201);
}
#endif