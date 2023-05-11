#include "driver/gpio.h"
#include "sensors/bh1750.h"

#define LED_PIN GPIO_NUM_26

float BRIGHTNESS = 1234.56f;
float TEMPERATURE = 25.5f;
float HUMIDITY = 30.00f;

int s_led_state = 0;

void init_sensors() {
    i2c_master_init();
}

void read_allSensor_Data() {
    //here read all Sensor data and write to valus
    printf("Reading sensors data...\n");
    
    s_led_state = !s_led_state;
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);        
    gpio_set_level(LED_PIN, s_led_state);

    BRIGHTNESS = get_brightness();
    TEMPERATURE += 0.01f;
    HUMIDITY += 0.1f;
}