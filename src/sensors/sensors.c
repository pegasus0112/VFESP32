#include "driver/gpio.h"
#include "sensors/bh1750.h"
#include "sensors/dht11.h"

#define LED_PIN GPIO_NUM_26
#define DHT11_PIN GPIO_NUM_21

float BRIGHTNESS = 1234.56f;
float TEMPERATURE = 0.0f;
float HUMIDITY = 0.0f;

int s_led_state = 0;

void init_sensors() {
    i2c_master_init();
    DHT11_init(DHT11_PIN);
}

void read_allSensor_Data() {
    //here read all Sensor data and write to valus
    printf("Reading sensors data...\n");
    
    s_led_state = !s_led_state;
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);        
    gpio_set_level(LED_PIN, s_led_state);

    // read_dht();

    BRIGHTNESS = get_brightness();

    // TEMPERATURE = get_temperature();
    TEMPERATURE = DHT11_read().temperature;
    // HUMIDITY = get_humidity();
    HUMIDITY = DHT11_read().humidity;
}
void read_dht() {


    printf("Temperature is %.1f°C \n", DHT11_read().temperature);
    printf("Humidity is %.1f%%\n", DHT11_read().humidity);
    printf("Status code is %d\n", DHT11_read().status);
}