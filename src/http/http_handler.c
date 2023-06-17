#include "esp_event.h"
#include "esp_http_server.h"
#include "json/jsonHandler.h"
#include "control/control.h"
#include "sensors/sensors.h"
#include "control/led_routine.h"
#include "nvs/nvs_handler.h"
#include "regulation/regulation.h"

esp_err_t get_sensor_data_handler(httpd_req_t *req) {
    read_allSensor_Data();
    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_send(req, get_farm_status_as_json_string(), HTTPD_RESP_USE_STRLEN);
    printf("SENDING SENSOR DATA AS RESPONSE\n");
    return ESP_OK;
};

esp_err_t post_changeSettings_handler(httpd_req_t *req) {
    char req_body[100];
    httpd_req_recv(req, req_body, sizeof(req_body));

    int new_min_water_level = get_int_from_Json(req_body, "min_water");
    if (new_min_water_level)
    {
        printf("changing min_water_level\n");
        min_water_level = new_min_water_level;
        save_int_value_by_key("waterMinDist", min_water_level);
    }
    
    int new_max_water_level = get_int_from_Json(req_body, "max_water");
    if (new_max_water_level)
    {
        printf("changing max_water_level\n");
        max_water_level = new_max_water_level;
        save_int_value_by_key("waterMaxDist", max_water_level);
    }
    
    int new_led_strength = get_int_from_Json(req_body, "led_strength");
    if (new_led_strength)
    {
        printf("changing specified_led_strength\n");
        specified_led_strength = new_led_strength;
        save_int_value_by_key("specLedStrngth", specified_led_strength);
    }
    
    int new_temperature = get_int_from_Json(req_body, "temperature");
    if (new_temperature)
    {
        printf("changing specified_temperature\n");
        specified_temperature = new_temperature;
        save_int_value_by_key("specTemp", specified_temperature);
    }
    
    int new_humidity = get_int_from_Json(req_body, "humidity");
    if (new_humidity)
    {
        printf("changing specified_humidity\n");
        specified_humidity = new_humidity;
        save_int_value_by_key("specHum", specified_temperature);
    }
    
    int new_water_pump_duty = get_int_from_Json(req_body, "pump_duty");
    if (new_water_pump_duty)
    {
        printf("changing water_pump_duty\n");
        change_duty_pump(new_water_pump_duty);
        save_int_value_by_key("pumpDuty", new_water_pump_duty);
    }
    
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
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