#include "driver/gpio.h"
#include "sensors/bh1750.h"
#include "sensors/hcsr04.h"

//read bh1750 for why delay must be bigger 120ms
#define DELAY_BETWEEN_MEASSURE 150

float BRIGHTNESS = -1.0f;
float TEMPERATURE = -1.0f;
float HUMIDITY = -1.0f;
int WATER_DISTANCE = -1.0f;


void init_sensors()
{
    i2c_master_init();
    ultrasonic_init();
}


void read_allSensor_Data()
{
    // here read all Sensor data and write to valus
    printf("Reading sensors data...\n");

    BRIGHTNESS = get_brightness();
    TEMPERATURE += 0.01f;
    HUMIDITY += 0.1f;
    WATER_DISTANCE = get_distance_cm();

    printf("Distance: %d cm \n", WATER_DISTANCE);
}