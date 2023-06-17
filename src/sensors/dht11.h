#include "driver/gpio.h"

void DHT11_init();

void DHT11_read();

//getter current temperature value of dht
int getTemperature();

//getter current humidity value of dht
int getHumidity();