#include "SDenum.hpp"

#include "exceptions/Exceptions.hpp"

SDenum::SDenum() {
    if (sd_device_enumerator_new(&enumerator) < 0 || !enumerator)
        throw DeviceEnumeratorError("Failed to create device enumerator");
}

SDenum::~SDenum() {
    sd_device_enumerator_unref(enumerator);
}

void SDenum::applyFilter() {
    sd_device_enumerator_add_match_subsystem(enumerator, "block", 1);
    sd_device_enumerator_add_match_property(enumerator, "DEVTYPE", "partition");
}

SDdevView SDenum::first() const {
    return SDdevView(sd_device_enumerator_get_device_first(enumerator));
}

SDdevView SDenum::next() const {
    return SDdevView(sd_device_enumerator_get_device_next(enumerator));
}