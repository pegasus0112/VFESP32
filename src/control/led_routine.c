#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <rom/ets_sys.h>
#include "control/control.h"
#include "sensors/bh1750.h"

//read bh1750 for why delay must be bigger 120ms
#define DELAY_BETWEEN_MEASSURE 250

float DELTA_LEDS_PERCENT = 0;

float meassure_led_delta() {
    float new_delta_percent = 0;

    //powering off leds before test
    change_duty_led_red(0);
    change_duty_led_blue(0);

    //do strength delta test for every percent and adding it to new_delta
    for (int i = 1; i < 101; i++)
    {
        change_duty_led_red(i);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        float red_lux = get_brightness();
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        change_duty_led_red(0);
        
        change_duty_led_blue(i);
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        float blue_lux = get_brightness();
        vTaskDelay(DELAY_BETWEEN_MEASSURE / portTICK_PERIOD_MS);
        change_duty_led_blue(0);

        printf("diff: %f  \n", red_lux - blue_lux);
        printf("percent : %f  \n", 100*(blue_lux-red_lux)/blue_lux);
        new_delta_percent += 100-(100*(blue_lux-red_lux)/blue_lux);
    }

    //after changing strength for testing, setting leds strength to lowest
    change_duty_led_red(1);
    change_duty_led_blue(1);

    //new_delta is in percent
    // 100 values added in new_delta, divede by 100 to get average
    new_delta_percent = new_delta_percent / 100;

    printf("new delta: %f\n", new_delta_percent);

    DELTA_LEDS_PERCENT = new_delta_percent;

    return DELTA_LEDS_PERCENT;
}