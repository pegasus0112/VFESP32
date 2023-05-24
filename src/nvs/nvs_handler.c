#include <stdlib.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "nvs/nvs_handler.h"

void init_nvs_storage()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        printf("Error while initializing NVS: %s", esp_err_to_name(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
};

int32_t read_int_from_storage_by_key(char *key)
{
    nvs_handle_t storage_handler;
    int32_t int_value = 0;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &storage_handler);

    if (err != ESP_OK)
    {
        printf("Error while trying to open NVS for read: %s\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_get_i32(storage_handler, key, &int_value);

        switch (err)
        {
        case ESP_OK:
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("value of key not initialized\n");
            break;
        default:
            printf("Error %d whole trying to read from key %s\n", err, key);
        }
    }
    nvs_close(storage_handler);
    return int_value;
};

void save_int_value_by_key(char *key, int32_t value)
{
    nvs_handle_t storage_handler;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &storage_handler);

    err = nvs_set_i32(storage_handler, key, value);

    if (err != ESP_OK)
    {
        printf("Error while writing key %s to storage: %d", key, err);
    }

    // write changes to storage
    err = nvs_commit(storage_handler);

    if (err != ESP_OK)
    {
        printf("Error while nvs commit: %d", err);
    }
    nvs_close(storage_handler);
};

void save_float_value_by_key(char *key, float value)
{

    int len = snprintf(NULL, 0, "%.2f", value);
    char *converted_float = malloc(len + 1);
    snprintf(converted_float, len + 1, "%f", value);

    nvs_handle_t storage_handler;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &storage_handler);

    err = nvs_set_str(storage_handler, key, converted_float);

    if (err != ESP_OK)
    {
        printf("Error while writing key %s to storage: %d", key, err);
    }

    // write changes to storage
    err = nvs_commit(storage_handler);

    if (err != ESP_OK)
    {
        printf("Error while nvs commit: %d", err);
    }
    nvs_close(storage_handler);
};

float read_float_from_storage_by_key(char *key)
{
    nvs_handle_t storage_handler;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &storage_handler);

    if (err != ESP_OK)
    {
        printf("Error while trying to open NVS for read: %s\n", esp_err_to_name(err));
    }
    else
    {
        size_t size;
        nvs_get_str(storage_handler, key, NULL, &size);
        char *char_value = malloc(size);
        err = nvs_get_str(storage_handler, key, char_value, &size);

        switch (err)
        {
            case ESP_OK:
                return atof(char_value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("value of key not initialized\n");
                break;
            default:
                printf("Error %d whole trying to read from key %s\n", err, key);
        }
    }
    nvs_close(storage_handler);
    return 0;
};