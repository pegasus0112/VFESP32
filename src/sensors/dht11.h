#include "driver/gpio.h"

enum dht11_status {
    DHT11_CRC_ERROR = -2,
    DHT11_TIMEOUT_ERROR,
    DHT11_OK
};

struct dht_values {
    int temperature;
    int humidity;
};

void DHT11_init();

struct dht_values DHT11_read();