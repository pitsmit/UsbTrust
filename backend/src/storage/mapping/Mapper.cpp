#include "Mapper.hpp"

template <> Device Mapper::from<Device>(const Row &r) {
    return DeviceBuilder()
        .withValidTo(r.get<std::string>("valid_to"))
        .withId(r.get<std::uint64_t>("id"))
        .withInfo(from<DeviceInfo>(r))
        .build();
}

template <> DeviceInfo Mapper::from<DeviceInfo>(const Row &r) {
    return DeviceInfoBuilder()
        .withVendorId(r.get<std::string>("vendor_id"))
        .withProductId(r.get<std::string>("product_id"))
        .withSerial(r.get<std::string>("serial"))
        .withVendorName(r.get<std::string>("vendor_name"))
        .withProductName(r.get<std::string>("product_name"))
        .build();
}

template <> MountRecord Mapper::from<MountRecord>(const Row &r) {
    return MountRecordBuilder()
        .withId(r.get<std::uint64_t>("id"))
        .withDevNode(r.get<std::string>("dev_node"))
        .withMountPoint(r.get<std::string>("mount_point"))
        .withMode(MountMode::parse(r.get<std::string>("mode")))
        .withInfo(from<DeviceInfo>(r))
        .build();
}