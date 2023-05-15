#include "cJSON.h"
#include "sensors/sensors.h"
#include "control/control.h"
#include <string.h>

int get_int_from_Json(char* json_string, char* key) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *bfound = cJSON_GetObjectItemCaseSensitive(root,key);
    return atoi(cJSON_Print(bfound));
}


char* get_status_as_json_string() {
    cJSON *data = cJSON_CreateObject();

    cJSON *sensors = cJSON_CreateObject();
    cJSON_AddNumberToObject(sensors, "brightness", BRIGHTNESS);
    cJSON_AddNumberToObject(sensors, "temperature", TEMPERATURE);
    cJSON_AddNumberToObject(sensors, "humidity", HUMIDITY);
    cJSON_AddItemToObject(data, "sensors", sensors);
    
    cJSON *controlls = cJSON_CreateObject();
    cJSON_AddNumberToObject(controlls, "fanSpeedPercent", FAN_PERCENT);
    cJSON_AddNumberToObject(controlls, "pumpSpeedPercent", PUMP_PERCENT);
    cJSON_AddNumberToObject(controlls, "lightPowerPercent", LED_PERCENT);
    cJSON_AddItemToObject(data, "controlls", controlls);
    return cJSON_Print(data);
}