#include "esp_timer.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.h"

enum dht11_status {
    DHT11_CRC_ERROR = -2,
    DHT11_TIMEOUT_ERROR,
    DHT11_OK
};

struct dht_values {
    int temperature;
    int humidity;
};

static gpio_num_t dht_pin = 4;
static int64_t last_read_time = -2000000;
static struct dht_values values = {-1, -1};

static int waitOrTimeout(uint16_t microSeconds, int level) {
    int micros_ticks = 0;
    while(gpio_get_level(dht_pin) == level) { 
        if(micros_ticks++ > microSeconds) 
            return DHT11_TIMEOUT_ERROR;
        ets_delay_us(1);
    }
    return micros_ticks;
}

static void sendStartSignal() {
    gpio_set_direction(dht_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(dht_pin, 0);
    ets_delay_us(20 * 1000);
    gpio_set_level(dht_pin, 1);
    ets_delay_us(40);
    gpio_set_direction(dht_pin, GPIO_MODE_INPUT);
}

static int checkResponse() {
    /* Wait for next step ~80us*/
    if(waitOrTimeout(80, 0) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;

    /* Wait for next step ~80us*/
    if(waitOrTimeout(80, 1) == DHT11_TIMEOUT_ERROR) 
        return DHT11_TIMEOUT_ERROR;

    return DHT11_OK;
}

void DHT11_read() {
    /* Tried to sense too son since last read (dht11 needs ~2 seconds to make a new read) */
    if(esp_timer_get_time() - 2000000 < last_read_time) {
        printf("DHT needs more time, returning old values...\n");
    }

    last_read_time = esp_timer_get_time();

    uint8_t data[5] = {0,0,0,0,0};

    sendStartSignal();

    if(checkResponse() == DHT11_TIMEOUT_ERROR) {
        printf("DHT timeout, old values used...\n");
    }
        
    //handling response
    for(int i = 0; i < 40; i++) {

        if(waitOrTimeout(50, 0) == DHT11_TIMEOUT_ERROR) {
            printf("DHT timeout, old values used...\n");
        }
                
        if(waitOrTimeout(70, 1) > 28) {
            //check for receiving 1 
            data[i/8] |= (1 << (7-(i%8)));
        }
    }

    //checking for caluclation errors
    if(data[4] == (data[0] + data[1] + data[2] + data[3])) {
        //updating values
        values.temperature = data[2];
        values.humidity = data[0];
    } else {
        printf("DHT calculation error, old values used...\n");
    }
}

int getTemperature() {
    return values.temperature;
}

int getHumidity() {
    return values.humidity;
}