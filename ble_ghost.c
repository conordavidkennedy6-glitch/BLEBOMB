#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <bt/bt_service/bt.h>
#include "ble_payloads.h"

typedef enum {
    SubmenuIndexApple,
    SubmenuIndexAndroid,
    SubmenuIndexWindows,
    SubmenuIndexOthers,
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
    bool is_broadcasting;
    uint32_t packet_count;
    uint8_t selected_mode;
    int8_t current_rssi;
} BleGhostApp;

// Distance estimation function based on RSSI and TX Power (+4dBm baseline)
static float calculate_distance(int8_t rssi, int8_t tx_power) {
    int8_t measured_power = tx_power - 55; // 1-meter calibration offset
    if (rssi >= -40) return 0.5f;
    if (rssi <= -90) return 15.0f;
    return (float)(-40 - rssi) * 0.2f + 0.5f;
}

static void ble_ghost_update_widget(BleGhostApp* app) {
    widget_reset(app->widget);
    
    float distance = calculate_distance(app->current_rssi, 4);
    
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "\e#BLE Ghost Pairing (%s)\n", BLE_MODES[app->selected_mode].category_name);
    widget_add_string_element(app->widget, 0, 0, AlignLeft, AlignTop, FontPrimary, buffer);

    snprintf(buffer, sizeof(buffer), "Status: %s", app->is_broadcasting ? "Active (Spamming)" : "Idle");
    widget_add_string_element(app->widget, 0, 16, AlignLeft, AlignTop, FontSecondary, buffer);

    snprintf(buffer, sizeof(buffer), "Packets Sent: %lu", app->packet_count);
    widget_add_string_element(app->widget, 0, 30, AlignLeft, AlignTop, FontSecondary, buffer);

    snprintf(buffer, sizeof(buffer), "Est. Distance: %.1f meters (RSSI: %d dBm)", (double)distance, app->current_rssi);
    widget_add_string_element(app->widget, 0, 44, AlignLeft, AlignTop, FontSecondary, buffer);
}

static void submenu_callback(void* context, uint32_t index) {
    BleGhostApp* app = context;
    app->selected_mode = index;
    app->is_broadcasting = true;
    app->packet_count = 0;
    app->current_rssi = -65; // Simulated active RSSI reading
    
    ble_ghost_update_widget(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewWidget);
}

static uint32_t view_submenu_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

static uint32_t view_widget_exit(void* context) {
    BleGhostApp* app = context;
    app->is_broadcasting = false;
    return ViewSubmenu;
}

static BleGhostApp* ble_ghost_app_alloc() {
    BleGhostApp* app = malloc(sizeof(BleGhostApp));
    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->submenu = submenu_alloc();
    submenu_add_item(app->submenu, "Apple Devices", SubmenuIndexApple, submenu_callback, app);
    submenu_add_item(app->submenu, "Android Devices", SubmenuIndexAndroid, submenu_callback, app);
    submenu_add_item(app->submenu, "Windows Devices", SubmenuIndexWindows, submenu_callback, app);
    submenu_add_item(app->submenu, "Others / Samsung", SubmenuIndexOthers, submenu_callback, app);
    view_set_previous_callback(submenu_get_view(app->submenu), view_submenu_exit);
    view_dispatcher_add_view(app->view_dispatcher, ViewSubmenu, submenu_get_view(app->submenu));

    app->widget = widget_alloc();
    view_set_previous_callback(widget_get_view(app->widget), view_widget_exit);
    view_dispatcher_add_view(app->view_dispatcher, ViewWidget, widget_get_view(app->widget));

    app->is_broadcasting = false;
    app->packet_count = 0;
    app->selected_mode = 0;
    app->current_rssi = -70;

    return app;
}

static void ble_ghost_app_free(BleGhostApp* app) {
    view_dispatcher_remove_view(app->view_dispatcher, ViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, ViewWidget);
    submenu_free(app->submenu);
    widget_free(app->widget);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
    free(app);
}

int32_t ble_ghost_app(void* p) {
    UNUSED(p);
    BleGhostApp* app = ble_ghost_app_alloc();
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewSubmenu);
    view_dispatcher_run(app->view_dispatcher);
    ble_ghost_app_free(app);
    return 0;
}
