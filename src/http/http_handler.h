#include "esp_http_server.h"

//post handler for receiving new fan duty
//parsing fan duty from json
//changing dan duty
esp_err_t post_fan_duty_handler(httpd_req_t *req);

//post handler for receiving new fan duty
//parsing fan duty from json
//changing dan duty
esp_err_t post_pump_duty_handler(httpd_req_t *req);

//post handler for receiving new fan duty
//parsing fan duty from json
//changing dan duty
esp_err_t post_led_duty_handler(httpd_req_t *req);

//get handler for reading current sensor data & sending them as response
esp_err_t get_sensor_data_handler(httpd_req_t *req);


//do delta meassurement of red blue leds
esp_err_t get_led_delta_calculation_handler(httpd_req_t *req);