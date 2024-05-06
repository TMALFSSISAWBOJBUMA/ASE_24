#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <pinout.h>
#include <ultrasonic.h>
#include <dht11.h>
#include <esp_err.h>


#define MAX_DISTANCE_CM 500 // 5m max

float front_distance = 1.0;  
float humidity, temperature = 23.0;

void measure_distance(void *pvParameters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_PIN,
        .echo_pin = ECHO_PIN
    };

    ultrasonic_init(&sensor);

    while (true)
    {
        float front_distance;
        esp_err_t res = ultrasonic_measure_temp_compensated(&sensor, MAX_DISTANCE_CM, &front_distance, temperature);
        if (res != ESP_OK)
        {
            printf("Error %d: ", res);
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        // else
            // printf("Distance: %0.04f cm\n", distance*100);
        
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void measure_environment(void *pvParameters)
{
    while (true)
    {
        esp_err_t res = dht_read_float_data(DHT_TYPE_DHT11, DHT_PIN, &humidity, &temperature);
        if (res != ESP_OK)
        {
            printf("DHT error %d: ", res);
            switch (res)
            {
                case ESP_ERR_TIMEOUT:
                    printf("Timeout occurred\n");
                    break;
                case ESP_ERR_INVALID_CRC:
                    printf("Checksum failed, invalid data received from sensor\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        // else
        //     printf("DHT Humidity: %.2f%%, Temperature: %.2f°C\n", humidity, temperature);
        
        vTaskDelay(pdMS_TO_TICKS(2000)); // Czekaj 2 sekundy przed kolejnym pomiarem
    }
}

void app_main()
{
    xTaskCreate(measure_distance, "dist_meas", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(measure_environment, "env_meas", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}
