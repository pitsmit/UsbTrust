#pragma once

#include "Device.hpp"
#include "DeviceInfo.hpp"
#include "RepositoryBase.hpp"
#include "DeviceInfoRepository.hpp"

#include <vector>
#include <string>

class DeviceRepository : RepositoryBase {
private:
    DeviceInfoRepository& info_rep;

    static Device mapDevice(char** v) {
        DeviceInfoBuilder info;
        if (v[1]) info.withVendorId(v[1]);
        if (v[2]) info.withProductId(v[2]);
        if (v[3]) info.withSerial(v[3]);
        if (v[4]) info.withProductName(v[4]);
        if (v[5]) info.withVendorName(v[5]);
        DeviceBuilder device;
        if (v[6]) device.withValidTo(v[6]);
        return device
            .withId(std::stoull(v[0]))
            .withInfo(info.build())
            .build();
    }

public:
    explicit DeviceRepository(
        DBConnection& connection,
        DeviceInfoRepository& infoRepo)
        : RepositoryBase(connection),
        info_rep(infoRepo)
    {}

    int add(const DeviceInfo& dev) {
        int deviceInfoId = info_rep.ensure(dev);
        static constexpr auto sql = "INSERT INTO Device (deviceInfoId, validTo) VALUES (?, NULL);";
        db.execute(sql, deviceInfoId);
        return db.lastInsertId();
    }

    std::vector<Device> getAll() {
        static constexpr auto sql =
            "SELECT d.id, di.vendorId, di.productId, di.serial, "
            "di.productName, di.vendorName, d.validTo "
            "FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id;";

        return db.queryAll<Device>(
            sql,
            mapDevice
        );
    }

    void remove(size_t id) {
        db.execute("DELETE FROM Device WHERE id = ?", id);
    }

    void updateValidTo(size_t id, std::optional<std::string> validTo) {
        db.execute("UPDATE Device SET validTo = ? WHERE id = ?",
        sqlValue(validTo), id);
    }

    std::optional<int> findActiveId(const DeviceInfo& info) {
        static constexpr auto sql =
            "SELECT d.id FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id "
            "WHERE di.vendorId = ? AND "
            "di.productId = ? AND "
            "di.serial = ?"
            " AND (d.validTo IS NULL "
            "OR d.validTo >= date('now'))"
            " LIMIT 1;";

        return db.queryScalar<int>(
            sql,
            info.vendorId,
            info.productId,
            info.serial
        );
    }
};