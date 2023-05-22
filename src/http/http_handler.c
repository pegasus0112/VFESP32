#include "esp_event.h"
#include "esp_http_server.h"
#include "json/jsonHandler.h"
#include "control/control.h"
#include "sensors/sensors.h"
#include "control/led_routine.h"

esp_err_t post_fan_duty_handler(httpd_req_t *req) {
    char req_body[20];
    httpd_req_recv(req, req_body, sizeof(req_body));

    int new_duty = get_int_from_Json(req_body, "fan_duty");
    httpd_resp_send(req, change_duty_fan(new_duty), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

esp_err_t post_pump_duty_handler(httpd_req_t *req) {
    char req_body[20];
    httpd_req_recv(req, req_body, sizeof(req_body));

    int new_duty = get_int_from_Json(req_body, "pump_duty");
    httpd_resp_send(req, change_duty_pump(new_duty), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

esp_err_t post_led_duty_handler(httpd_req_t *req) {
    char req_body[20];
    httpd_req_recv(req, req_body, sizeof(req_body));
    
    int new_duty_red = get_int_from_Json(req_body, "led_duty_red");
    change_duty_led_red(new_duty_red);

    //apply delta percentage
    //change_duty_led_red(new_duty_red * delta);

    httpd_resp_send(req, "", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

esp_err_t get_sensor_data_handler(httpd_req_t *req) {
    read_allSensor_Data();
    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_send(req, get_sensor_status_as_json_string(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

esp_err_t get_led_delta_calculation_handler(httpd_req_t *req) {
    char float_string[10];

    sprintf(float_string, "%f", meassure_led_delta());
    httpd_resp_send(req, float_string, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};