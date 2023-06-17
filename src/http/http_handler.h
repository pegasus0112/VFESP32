#include "esp_http_server.h"

/**
 * get handler for reading current sensor data
 * reading all sensors (update values)
 * sending new sensor data as response
*/
esp_err_t get_sensor_data_handler(httpd_req_t *req);

/**
 * get handler for updating delta of red blue leds difference
 * triggers led_routine for measuring new delta
*/
esp_err_t get_led_delta_calculation_handler(httpd_req_t *req);

/**
 * post handler for changing by user changable values
 * setting;
 * min_water_level
 * max_water_level
 * specified_led_strength
 * specified_temperature
 * specified_humidity
 * pump_duty
 */
esp_err_t post_changeSettings_handler(httpd_req_t *req);
