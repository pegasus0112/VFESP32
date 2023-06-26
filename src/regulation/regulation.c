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
#include "esp_timer.h"

state current_state = STARTING;

int min_water_level = 10;

int max_water_level = 20;

int specified_temperature = 20;

//temperature deviation (parsley) of +- 7 degrees (--> 13-27 degrees)
// >2 because accuracy of dht11 is +-2 degree
int temperature_deviation = 7;


int specified_humidity = 50;

//init fan-off-timer to regulate the humidity system after x seconds
const esp_timer_create_args_t fan_timer_off = {
    .callback = &set_fan_to_standard,
    .name = "humidity regulation"
};

esp_timer_handle_t fan_off_handler;

//humidity deviation for parsley of +- 10% at 50% optimum
int humidity_deviation = 10;

int specified_led_strength = 3000;

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

void init_timer_fcts(){

    //init timer handler for fan off timer
    ESP_ERROR_CHECK(esp_timer_create(&fan_timer_off, &fan_off_handler));
}

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

void regulate_fan_based_on_temperature_and_humidity()
{
    printf("Temperature: %d, Optimum: %d \n", TEMPERATURE, specified_temperature);
    printf("Humidity: %d, Optimum: %d \n", HUMIDITY, specified_humidity);


    if(timer_measurement >= 600){ //10min (600s)
        AVERAGE_HUMIDITY = HUMIDITY_SUM / timer_measurement;
        printf("AVG Hum: %d \n", AVERAGE_HUMIDITY);
        if(AVERAGE_HUMIDITY > 75){
            
            change_duty_fan(100); //for x minutes
            ESP_ERROR_CHECK(esp_timer_start_once(fan_off_handler, 180000000)); //3 Min | 180 s
          
        } 
        HUMIDITY_SUM = 0;
        timer_measurement = 0;
    }

    if(TEMPERATURE <= (specified_temperature + temperature_deviation) && TEMPERATURE >= (specified_temperature - temperature_deviation)){
        printf("Temperature OK! \n");
        change_duty_fan(30);
        return;
    }

    if(TEMPERATURE > specified_temperature + temperature_deviation){
        printf("Temperature too high! \n");
        change_duty_fan(100);
    }else if(TEMPERATURE < specified_temperature - temperature_deviation){
        printf("Temperature too low! \n");
        change_duty_fan(0);
    }
    
}

/**
 * regulation for activating refill water pump if water level is too low
 * deactivate if it reaches the max_water_level
 */
void regulate_refill_pump_based_on_ultrasonic_distance()
{
    if (WATER_DISTANCE >= min_water_level)
    {
        printf("refilling!\n");
        set_state_pump_refill(true);
    }
    else if (WATER_DISTANCE <= max_water_level)
    {
        printf("water is at max level\n");
        set_state_pump_refill(false);
    }
    
    printf("distance: %d refillPump status: %s\n",WATER_DISTANCE, PUMP_REFILL_ON == true ? "ON" : "OFF");
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

        timer_measurement += 1;
        printf("timer: %d \n", timer_measurement);
        HUMIDITY_SUM += HUMIDITY;
        printf("humidity sum: %d \n", HUMIDITY_SUM);

        regulate_leds_based_on_light();
        regulate_fan_based_on_temperature_and_humidity();
        regulate_refill_pump_based_on_ultrasonic_distance();
        break;
    }
}
