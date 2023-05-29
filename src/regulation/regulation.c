#include <stdio.h>
#include "utils.h"
#include <stdbool.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "regulation.h"
#include "control/control.h"
#include "control/led_routine.h"
#include "sensors/sensors.h"
#include "sensors/bh1750.h"
#include "nvs/nvs_handler.h"
#include "http/http_server.h"

state current_state = STARTING;

int min_water_level = 10;

int max_water_level = 20;


int specified_led_strength = 3000;


float specified_temperature = 25.0f;

/**
 * deviation because regulate_leds_based_on_light() will never reach exact specified_led_strength value
 * regualtion based on approximation to specified_led_strength +- deviation
 */
int leds_allowed_deviation = 15;

/**
 * multiplier for led strength change
 * led sterngth change is thereby potentially and not linearly
 */
float led_multiplier = 1;

/**
 * regulating red & blue leds
 * light sensor read current strength of light and changes
 * strength of leds so it comes close to specified_led_strength
 */
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

    printf("setting red value: %f!\n", new_led_percent);
    change_duty_led_red(new_led_percent);
    printf("setting blue value: %f!\n", (new_led_percent - delta_leds) * (blue_proportion_percent / 100));
    change_duty_led_blue((new_led_percent - delta_leds) * (blue_proportion_percent / 100));

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

// WILL BE REMOVED
void regulate_fan_based_on_light()
{
    printf("brightness: %f \n", BRIGHTNESS);
    change_duty_fan(remap_float_to_range(BRIGHTNESS, 0, 1000, 0, 100));
}

void regulate_fan_based_on_temperature()
{
    // TODO
}

/**
 * regulation for activating refill water pump if water level is too low
 * deactivate if it reaches the max_water_level
 */
void regulate_refill_pump_based_on_ultrasonic_distance()
{

    if (!PUMP_REFILL_ON && WATER_DISTANCE <= min_water_level)
    {
        set_state_pump_refill(true);
    }
    else if (PUMP_REFILL_ON && WATER_DISTANCE >= max_water_level)
    {
        set_state_pump_refill(false);
    }
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
    case OK:
        read_allSensor_Data();
        regulate_leds_based_on_light();
        regulate_fan_based_on_light();
        regulate_refill_pump_based_on_ultrasonic_distance();
        break;
    }
}