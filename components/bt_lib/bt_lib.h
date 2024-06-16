#ifndef BT_LIB_H
#define BT_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_spp_api.h>
#include <esp_gap_bt_api.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_log.h>

#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "ESP32_BT"

void bt_init();
void bt_send_data(const char *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* BT_LIB_H */
