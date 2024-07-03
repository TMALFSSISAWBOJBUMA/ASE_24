#ifndef BT_COMMS_H
#define BT_COMMS_H

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include "freertos/queue.h"

#define SPP_TAG "BT_SPP"
#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "R4-EIT"
#define MAX_MSG_LEN 30

void bt_comms_init(QueueHandle_t recv_q);

esp_err_t bt_comms_send(const char *msg);

/**
 * @brief   Use:
 *          QueueHandle_t q = xQueueCreate(5, MAX_MSG_LEN);
 *          bt_comms_init(q);
 * 
 *          xTaskCreate(test_bt_comms, "bt_comms", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
 * 
 * @param pvParameters unused
 */
void test_bt_comms(void *pvParameters);

#endif