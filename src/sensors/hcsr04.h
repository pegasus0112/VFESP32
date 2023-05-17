#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <rom/ets_sys.h>

#define ECHO_PIN 25
#define TRIGGER_PIN 26

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define TIMEOUT 6000
#define ROUNDTRIP_CM 58

#define check_timeout(start, len) ((esp_timer_get_time() - (start)) >= (len))

//lock for blocking ISR inside time relvant code block
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

static void ultrasonic_init()
{
    gpio_set_direction(TRIGGER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_level(TRIGGER_PIN, 0);
}

static esp_err_t ultrasonic_measure_raw(uint32_t max_time_us, uint32_t *time_us)
{
    //blocking ISR
    portENTER_CRITICAL(&mux);
    int64_t time = 0;

    // Set TRIGGER_PIN for TRIGGER_LOW_DELAY us low
    gpio_set_level(TRIGGER_PIN, 0);
    ets_delay_us(TRIGGER_LOW_DELAY);

    // Set TRIGGER_PIN for TRIGGER_HIGH_DELAY us high
    gpio_set_level(TRIGGER_PIN, 1);
    ets_delay_us(TRIGGER_HIGH_DELAY);

    gpio_set_level(TRIGGER_PIN, 0);

    // checking if previous high is over
    //if not over, ECHO_PIN == high
    if (gpio_get_level(ECHO_PIN))
        return ESP_FAIL;

    //save time of sending signal
    int64_t start_time = esp_timer_get_time();

    // wait for echo to return
    while (!gpio_get_level(ECHO_PIN))
    {
        //echo did not return in time
        if (check_timeout(start_time, TIMEOUT))
            return ESP_FAIL;
    }

    // meassure time of echo ping
    int64_t echo_start = esp_timer_get_time();

    while (gpio_get_level(ECHO_PIN))
    {
        time = esp_timer_get_time();
        // check if echo took to long
        if (check_timeout(echo_start, max_time_us))
            return ESP_FAIL;
    }

    //release ISR block because time is not relevant anymore
    portEXIT_CRITICAL(&mux);

    *time_us = time - echo_start;
    return ESP_OK;
}

// returns cm distance meassured by ultrasonic sensor
// 4m max distance
static int get_distance_cm()
{
    uint32_t time_us;
    //if no check, spinlock won´t be released!
    if (ultrasonic_measure_raw(400 * ROUNDTRIP_CM, &time_us) != ESP_OK)
    {
        portEXIT_CRITICAL(&mux);
        printf("Error while reading ultrasonic sensor! \n Please check that sensor is nocht blocked, or range to wide");
        return -1;
    }
    return time_us / ROUNDTRIP_CM;
}