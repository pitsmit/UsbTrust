#pragma once

#include "entities/Device.hpp"
#include "entities/DeviceInfo.hpp"
#include "RepositoryBase.hpp"
#include "DeviceInfoRepository.hpp"
#include "mappers/DeviceMapper.hpp"

#include <vector>

class DeviceRepository : public RepositoryBase {
private:
    DeviceInfoRepository info_rep;

public:
    explicit DeviceRepository(
        DBConnection& connection)
        : RepositoryBase(connection),
        info_rep(connection)
    {}

    core::Id add(const DeviceInfo& dev) {
        auto deviceInfoId = info_rep.ensure(dev);
        static constexpr auto sql = 
            "INSERT INTO Device (deviceInfoId, validTo) VALUES (?, NULL) RETURNING id;";
        auto id = db.queryScalar<core::Id>(sql, deviceInfoId);
        return *id;
    }

    std::vector<Device> getAll() {
        static constexpr auto sql =
            "SELECT d.id, di.vendorId, di.productId, di.serial, "
            "di.productName, di.vendorName, d.validTo "
            "FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id;";

        return db.queryAll<Device>(
            sql,
            DeviceMapper::fromRow
        );
    }

    void remove(core::Id id) {
        static constexpr auto sql = "DELETE FROM Device WHERE id = ?";
        db.execute(sql, id);
    }

    void updateValidTo(core::Id id, std::optional<std::string> validTo) {
        static constexpr auto sql = 
            "UPDATE Device SET validTo = ? WHERE id = ?";
        db.execute(sql, validTo, id);
    }

    std::optional<core::Id> findActiveId(const DeviceInfo& info) {
        static constexpr auto sql =
            "SELECT d.id FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id "
            "WHERE di.vendorId = ? AND "
            "di.productId = ? AND "
            "di.serial = ?"
            " AND (d.validTo IS NULL "
            "OR d.validTo >= date('now'))"
            " LIMIT 1;";

        return db.queryScalar<core::Id>(
            sql,
            info.vendorId,
            info.productId,
            info.serial
        );
    }
};