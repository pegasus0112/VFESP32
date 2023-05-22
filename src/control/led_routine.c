#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <rom/ets_sys.h>
#include "control/control.h"
#include "sensors/bh1750.h"

//read bh1750 for why delay must be bigger 120ms
#define DELAY_BETWEEN_MEASSURE 250

float meassure_led_delta() {
    float delta = 0;
    change_duty_led_red(0);
    change_duty_led_blue(0);

    for (int i = 1; i < 101; i++)
    {
        printf("step: %d \n", i);
        change_duty_led_red(i);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        float red_lux = get_brightness();
        printf("red: %f  ????????????????????????????????????????????????\n", red_lux);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        change_duty_led_red(0);
        
        change_duty_led_blue(i);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        float blue_lux = get_brightness();
        printf("blue: %f  ????????????????????????????????????????????????\n", blue_lux);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        change_duty_led_blue(0);

        printf("diff: %f  \n", red_lux - blue_lux);
        printf("percent : %f  \n", (100/blue_lux)*red_lux -100);
        delta += (100/blue_lux)*red_lux -100;
    }

    change_duty_led_red(1);
    change_duty_led_blue(1);

    printf("difference: %f  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", delta / 100);
    return delta / 100;
}