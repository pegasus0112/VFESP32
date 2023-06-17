#include "cJSON.h"
#include "sensors/sensors.h"
#include "control/control.h"
#include <string.h>
#include "regulation/regulation.h"
#include "control/led_routine.h"
#include <stdio.h>

int get_int_from_Json(char *json_string, char *key)
{
    cJSON *root = cJSON_Parse(json_string);
    cJSON *bfound = cJSON_GetObjectItemCaseSensitive(root, key);

    if (bfound)
    {
        return atoi(cJSON_Print(bfound));
    }
    return NULL;
}

char *get_string_from_Json(char *json_string, char *key)
{
    cJSON *root = cJSON_Parse(json_string);
    cJSON *bfound = cJSON_GetObjectItemCaseSensitive(root, key);
    if (bfound)
    {
        return cJSON_Print(bfound);
    }
    return NULL;
}

char *get_farm_status_as_json_string()
{
    cJSON *data = cJSON_CreateObject();

    cJSON *sensors = cJSON_CreateObject();
    cJSON_AddNumberToObject(sensors, "brightness", BRIGHTNESS);
    cJSON_AddNumberToObject(sensors, "specTemperature", specified_temperature);
    cJSON_AddNumberToObject(sensors, "temperature", TEMPERATURE);
    cJSON_AddNumberToObject(sensors, "specHumidity", specified_humidity);
    cJSON_AddNumberToObject(sensors, "humidity", HUMIDITY);
    cJSON_AddNumberToObject(sensors, "ultrasonicDist", WATER_DISTANCE);
    cJSON_AddItemToObject(data, "sensors", sensors);

    cJSON *controlls = cJSON_CreateObject();
    cJSON_AddNumberToObject(controlls, "fanSpeedPercent", FAN_PERCENT);
    cJSON_AddNumberToObject(controlls, "pumpSpeedPercent", PUMP_PERCENT);
    cJSON_AddNumberToObject(controlls, "refillPumpState", PUMP_REFILL_ON);

    cJSON_AddNumberToObject(controlls, "specLedStrgth", specified_led_strength);
    cJSON_AddNumberToObject(controlls, "lightPowerPercentRed", LED_RED_PERCENT);
    cJSON_AddNumberToObject(controlls, "lightPowerPercentBlue", LED_BLUE_PERCENT);

    cJSON_AddNumberToObject(controlls, "ultrasonicMinDistance", min_water_level);
    cJSON_AddNumberToObject(controlls, "ultrasonicMaxDistance", max_water_level);

    cJSON_AddNumberToObject(controlls, "deltaLeds", delta_leds);

    cJSON_AddItemToObject(data, "controlls", controlls);
    return cJSON_Print(data);
}