#include "esp_event.h"
#include "esp_http_server.h"
#include "json/jsonHandler.h"
#include "control/control.h"
#include "sensors/sensors.h"
#include "control/led_routine.h"
#include "nvs/nvs_handler.h"
#include "regulation/regulation.h"

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

esp_err_t get_sensor_data_handler(httpd_req_t *req) {
    read_allSensor_Data();
    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_send(req, get_farm_status_as_json_string(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

esp_err_t get_led_delta_calculation_handler(httpd_req_t *req) {
    //disable regulation so testing does not influence it
    current_state = MEASURING;

    //first send response, because measurement takes time
    httpd_resp_send(req, "starting delta measurement", HTTPD_RESP_USE_STRLEN);

    float delta = meassure_led_delta();
    
    printf("old delta: %f\n", read_float_from_storage_by_key("delta_led"));

    printf("saving new value: %f\n", delta );
    save_float_value_by_key("delta_led", delta);

    printf("getting new value: %f\n", delta );
    printf("new delta: %f\n", read_float_from_storage_by_key("delta_led"));

    current_state = OK;
    return ESP_OK;
};

esp_err_t post_led_strength_handler(httpd_req_t *req) {
    char req_body[20];
    httpd_req_recv(req, req_body, sizeof(req_body));

    printf("changing specified strength!\n");

    //specified_led_strength = get_int_from_Json(req_body, "led_strength");

    specified_led_strength = atof(req_body);

    httpd_resp_send(req, "changed LEDs strength", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}