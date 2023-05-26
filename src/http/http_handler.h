#include "esp_http_server.h"

/**
 * post handler for receiving new fan duty
 * parsing fan duty from json
 * changing fan duty
*/
esp_err_t post_fan_duty_handler(httpd_req_t *req);

/**
 * post handler for receiving new fan pump
 * parsing pump duty from json
 * changing pump duty
*/
esp_err_t post_pump_duty_handler(httpd_req_t *req);

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
 * post handler for setting the specified led strenght by the user
 * setting specified_led_strength by post body
*/
esp_err_t post_led_strength_handler(httpd_req_t *req);