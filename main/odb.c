#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <pinout.h>
#include <ultrasonic.h>
#include <dht11.h>
#include <esp_err.h>
#include <drivetrain.h>
#include <driver/gpio.h>
#include <bt_lib.h>

motor_struct_t left = {.state = STOP, .duty_fix = {1.0, 1.0}}, right = {.state = STOP, .duty_fix = {1.0, 1.0}};

#define MAX_DISTANCE_CM 500 // 5m max

float front_distance = 1.0;
float humidity, temperature = 23.0;

#define odb_prawy_przod 4
#define odb_lewy_przod 4
#define odb_prawy_bok 4
#define odb_lewy_bok 4

void measure_distance(void *pvParameters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_PIN,
        .echo_pin = ECHO_PIN};

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

void reflection_sensor()
{
    int prawy_przod = gpio_get_level(odb_prawy_przod);
    int lewy_przod = gpio_get_level(odb_lewy_przod);
    int prawy_bok = gpio_get_level(odb_prawy_bok);
    int lewy_bok = gpio_get_level(odb_lewy_bok);
}


static void set_motors(float duty, bool forward){
    if(forward){
        motor_forward(&left, duty);
        motor_forward(&right, duty);
    } else {
        motor_backward(&left, duty);
        motor_backward(&right, duty);
    }
}

void algorithm(void *pvParameters)
{
    initialize_motors(&left, &right);
    while (1)
    {
        set_motors(20, true);
        vTaskDelay(pdMS_TO_TICKS(2000));
        set_motors(0, true);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_motors(20, false);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}




void app_main()
{
    // Inicjalizacja Bluetootha z biblioteki
    bt_init();

    // Tworzenie zadań do pomiaru odległości, środowiska i algorytmu
    xTaskCreate(measure_distance, "dist_meas", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(measure_environment, "env_meas", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(algorithm, "algorithm", configMINIMAL_STACK_SIZE * 3, NULL, 4, NULL);

    while (1) {
        // Odczytaj stany odbiciowe
        int prawy_przod = gpio_get_level(odb_prawy_przod);
        int lewy_przod = gpio_get_level(odb_lewy_przod);
        int prawy_bok = gpio_get_level(odb_prawy_bok);
        int lewy_bok = gpio_get_level(odb_lewy_bok);

        // Przygotuj dane do wysłania przez Bluetooth
        char data[64];
        snprintf(data, sizeof(data), "Sensors: %d,%d,%d,%d\n",
                 prawy_przod, lewy_przod, prawy_bok, lewy_bok);

        // Wyślij dane przez Bluetooth
        bt_send_data(data, strlen(data));

        // Opóźnienie na 2 sekundy (2000 ms)
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}




