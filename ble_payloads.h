#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char* name;
    const uint8_t* data;
    size_t size;
    int8_t tx_power; // TX Power in dBm (+4dBm)
} BlePayload;

typedef struct {
    const char* category_name;
    const BlePayload* payloads;
    size_t count;
} BleMode;

static const uint8_t AIRPODS_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x01, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t FASTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x2c, 0xfe, 0x06, 0x16, 0x2c, 0xfe, 0x80, 0x83, 0x00
};

static const uint8_t SWIFTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x03, 0x00, 0x12, 0x16, 0x03, 0x00, 0x80, 0x01, 0x00
};

static const uint8_t SAMBUDS_PAYLOAD[] = {
    0x05, 0x16, 0x12, 0x01, 0x00, 0x00
};

static const BlePayload APPLE_PAYLOADS[] = {
    {"Apple AirPods", AIRPODS_PAYLOAD, sizeof(AIRPODS_PAYLOAD), 4}
};

static const BlePayload ANDROID_PAYLOADS[] = {
    {"Google Fast Pair", FASTPAIR_PAYLOAD, sizeof(FASTPAIR_PAYLOAD), 4}
};

static const BlePayload WINDOWS_PAYLOADS[] = {
    {"Microsoft Swift Pair", SWIFTPAIR_PAYLOAD, sizeof(SWIFTPAIR_PAYLOAD), 4}
};

static const BlePayload OTHERS_PAYLOADS[] = {
    {"Samsung Buds", SAMBUDS_PAYLOAD, sizeof(SAMBUDS_PAYLOAD), 4}
};

static const BleMode BLE_MODES[] = {
    {"Apple Devices", APPLE_PAYLOADS, 1},
    {"Android Devices", ANDROID_PAYLOADS, 1},
    {"Windows Devices", WINDOWS_PAYLOADS, 1},
    {"Others / Samsung", OTHERS_PAYLOADS, 1}
};
