#include "driver/gpio.h"
#include "sensors/bh1750.h"
#include "sensors/hcsr04.h"
#include "sensors/dht11.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

//read bh1750 for why delay must be bigger 120ms
#define DELAY_BETWEEN_MEASSURE 150

float BRIGHTNESS = -1.0f;
int TEMPERATURE = 0.0f;
int HUMIDITY = 0.0f;
//int HUMIDITY_SUM = 0;
//int AVERAGE_HUMIDITY = 0;
int WATER_DISTANCE = -1.0f;

//Timer for measurements over a period of time
//int timer_measurement = 0;


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

    //timer_measurement += 1;

    BRIGHTNESS = get_brightness();
    WATER_DISTANCE = get_distance_cm();

    printf("Distance: %d cm \n", WATER_DISTANCE);

    TEMPERATURE = DHT11_read().temperature;
    HUMIDITY = DHT11_read().humidity;
    //HUMIDITY_SUM += HUMIDITY; 
    printf("Temp: %d \n", TEMPERATURE);
    printf("Hum: %d \n", HUMIDITY);
    //printf("timer: %d", timer_measurement);
    //printf("AVG HUM: %d", HUMIDITY_SUM);

}