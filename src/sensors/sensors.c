#include "driver/gpio.h"
#include "sensors/bh1750.h"
#include "sensors/hcsr04.h"
#include "sensors/dht11.h"

float BRIGHTNESS = -1.0f;
int TEMPERATURE = 0.0f;
int HUMIDITY = 0.0f;
int HUMIDITY_SUM = 0;
int AVERAGE_HUMIDITY = 0;
int WATER_DISTANCE = -1.0f;


//Timer for measurements over a period of time
int timer_measurement = 0;

void init_sensors()
{
    i2c_master_init();
    ultrasonic_init();
}


void read_allSensor_Data()
{
    // here read all Sensor data and write to values
    printf("Reading sensors data...\n");
    DHT11_read();

    BRIGHTNESS = get_brightness();
    WATER_DISTANCE = get_distance_cm();
    TEMPERATURE = getTemperature();
    HUMIDITY = getHumidity();
}