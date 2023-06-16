#include "driver/gpio.h"
#include "sensors/bh1750.h"
#include "sensors/hcsr04.h"
#include "sensors/dht11.h"

float BRIGHTNESS = -1.0f;
int TEMPERATURE = 0.0f;
int HUMIDITY = 0.0f;
int WATER_DISTANCE = -1.0f;

void init_sensors()
{
    i2c_master_init();
    ultrasonic_init();
    DHT11_init(GPIO_NUM_4);
}


void read_allSensor_Data()
{
    // here read all Sensor data and write to values
    printf("Reading sensors data...\n");

    BRIGHTNESS = get_brightness();
    WATER_DISTANCE = get_distance_cm();

    printf("Distance: %d cm \n", WATER_DISTANCE);

    TEMPERATURE = DHT11_read().temperature;
    HUMIDITY = DHT11_read().humidity;
    printf("Temp: %d \n", TEMPERATURE);
    printf("Hum: %d \n", HUMIDITY);
}