#include "driver/gpio.h"
#include "sensors/bh1750.h"

float BRIGHTNESS = 1234.56f;
float TEMPERATURE = 25.5f;
float HUMIDITY = 30.00f;

//initialze sensors so sensors can read data
void init_sensors() {
    i2c_master_init();
}

//reading all sensors & update attributes with new values
void read_allSensor_Data() {
    //here read all Sensor data and write to valus
    printf("Reading sensors data...\n");

    BRIGHTNESS = get_brightness();
    TEMPERATURE += 0.01f;
    HUMIDITY += 0.1f;
}