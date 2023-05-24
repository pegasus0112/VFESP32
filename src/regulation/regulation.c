#include <stdio.h>
#include "utils.h"
#include <stdbool.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "regulation.h"
#include "control/control.h"
#include "sensors/sensors.h"
#include "sensors/bh1750.h"
#include "nvs/nvs_handler.h"
#include "http/http_server.h"

state current_state = STARTING;

int specified_led_strength = 3000;

int leds_allowed_deviation = 15;
float led_multiplier = 1;

void regulate_leds_based_on_light()
{
    printf("checking brightness: %f \n", BRIGHTNESS);
    if (BRIGHTNESS >= (specified_led_strength - leds_allowed_deviation) && BRIGHTNESS <= (specified_led_strength + leds_allowed_deviation))
    {
        printf("brightness OK!\n");
        // led brightness is within allowed range
        led_multiplier = 1;
        return;
    }

    printf("brightness BAD!\n");

    led_multiplier *= 1.5f;
    printf("multiplier: %f\n", led_multiplier);

    bool isTooLow = get_brightness() < specified_led_strength;
    printf("Is too low: %s\n", isTooLow ? "true" : "false");

    float new_led_percent = 0;
    if (isTooLow)
    {
        new_led_percent = LED_RED_PERCENT + led_multiplier;

        if (new_led_percent > 100)
        {
            printf("MAX reached!\n");
            led_multiplier = 1;
            new_led_percent = 100;
        }
    }
    else
    {
        new_led_percent = LED_RED_PERCENT - led_multiplier;
        if (new_led_percent < 1)
        {
            printf("MIN reached!\n");
            led_multiplier = 1;
            new_led_percent = 1;
        }
    }

    change_duty_led_red(new_led_percent);
    update_proportion_led_blue_to_red();

    printf("Current Percent: %f\n", LED_RED_PERCENT);

    // wait so next light measurement will be accurate
    vTaskDelay(250 / portTICK_PERIOD_MS);

    BRIGHTNESS = get_brightness();
    if (isTooLow != (BRIGHTNESS <= specified_led_strength))
    {
        printf("Resetting multiplier\n");
        led_multiplier = 1;
    }
}

void regulate_fan_based_on_light()
{
    printf("brightness: %f \n", BRIGHTNESS);
    change_duty_fan(remap_float_to_range(BRIGHTNESS, 0, 1000, 0, 100));
}

void regulate_fan_based_on_temperature(){
    // TODO
}

void regulate_refill_pump_based_on_ultrasonic(){
    // TODO
}

void regulate()
{
    switch (current_state)
    {
    case STARTING:
        read_all_saved_data_from_nvs();
        networking_init();
        pwm_init();
        init_sensors();
        current_state = OK;
        break;
    case MEASURING:
        break;
    case REGULATE:
        break;
    case OK:
        read_allSensor_Data();
        regulate_leds_based_on_light();
        regulate_fan_based_on_light();
        break;
    }
}