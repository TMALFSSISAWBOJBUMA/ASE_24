#include "bt_lib.h"

static const char *TAG = "BT_CLASSIC_SPP";
static uint32_t spp_handle = 0;

static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    // Obsługa zdarzeń Bluetooth GAP
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
            char bda_str[18];
            esp_log_buffer_hex(TAG, param->disc_res.bda, ESP_BD_ADDR_LEN);
            ESP_LOGI(TAG, "Device found: %s", bda_str);
            break;
        }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
                ESP_LOGI(TAG, "Device discovery stopped.");
            } else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
                ESP_LOGI(TAG, "Device discovery started.");
            }
            break;
        }
        default:
            break;
    }
}

static void bt_app_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI(TAG, "ESP_SPP_INIT_EVT");
            esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME);
            break;
        case ESP_SPP_START_EVT:
            ESP_LOGI(TAG, "ESP_SPP_START_EVT");
            break;
        case ESP_SPP_DATA_IND_EVT:
            ESP_LOGI(TAG, "ESP_SPP_DATA_IND_EVT len=%d handle=%d", param->data_ind.len, param->data_ind.handle);
            esp_log_buffer_hex("", param->data_ind.data, param->data_ind.len);

            // Obsługa otrzymanych danych
            if (param->data_ind.len == 1) {
                char received_char = param->data_ind.data[0];
                if (received_char == '1') {
                    ESP_LOGI(TAG, "Received '1': Moving forward");
                    // Tutaj wywołujesz funkcje związane z uruchomieniem czegoś, np. rozpoczęcie ruchu robota
                    // np. start_robot_movement();
                } else if (received_char == '2') {
                    ESP_LOGI(TAG, "Received '2': Stopping");
                    // Tutaj wywołujesz funkcje związane ze zatrzymaniem czegoś, np. zatrzymanie ruchu robota
                    // np. stop_robot_movement();
                } else {
                    ESP_LOGI(TAG, "Received unexpected data: %c", received_char);
                }
            } else {
                ESP_LOGW(TAG, "Received unexpected data length: %d", param->data_ind.len);
            }
            break;
        default:
            break;
    }
}



void bt_init() {
    // Kod inicjalizacji Bluetootha
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    if (ret) {
        ESP_LOGE(TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "%s init bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    esp_bt_gap_register_callback(bt_app_gap_cb);
    esp_spp_register_callback(bt_app_spp_cb);
    esp_spp_init(ESP_SPP_MODE_CB);

    esp_bt_dev_set_device_name(DEVICE_NAME);
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
}

void bt_send_data(const char *data, size_t len) {
    if (spp_handle != 0) {
        esp_spp_write(spp_handle, len, (uint8_t *)data);
    }
}
