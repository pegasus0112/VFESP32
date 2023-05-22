#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_tls.h"
#include "esp_sleep.h"
#include "loginData.h"
#include "http_handler.h"

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    printf("checking wifi status... \n");
    switch (event_id)
    {
        case WIFI_EVENT_STA_START:
            printf("connecting \n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("connected \n");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            //esp_sleep_enable_timer_wakeup(20000000);
            //esp_light_sleep_start();
            printf("Retry connecting....\n");
            esp_wifi_connect();
            break;
        case IP_EVENT_STA_GOT_IP:
            printf("got IP \n");
            break;
        default:
            char char_buffer[20];
            sprintf(char_buffer, "%ld", event_id);
            printf("not handled case: %s\n", char_buffer);
            break;
    }
};

void wifi_init() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        }};

    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    esp_wifi_start();

    esp_wifi_connect();
};

void server_init() {
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server_handle = NULL;
    httpd_start(&server_handle, &server_config);

    httpd_uri_t uri_get_sensors = {
        .uri = "/status",
        .method = HTTP_GET,
        .handler = get_sensor_data_handler
    };

    httpd_uri_t uri_post_fan = {
        .uri = "/fan",
        .method = HTTP_POST,
        .handler = post_fan_duty_handler,
        .user_ctx = NULL
    };

    httpd_uri_t uri_post_pump = {
        .uri = "/pump",
        .method = HTTP_POST,
        .handler = post_pump_duty_handler,
        .user_ctx = NULL
    };

    httpd_uri_t uri_post_led = {
        .uri = "/led",
        .method = HTTP_POST,
        .handler = post_led_duty_handler,
        .user_ctx = NULL
    };

    httpd_uri_t uri_post_delta = {
        .uri = "/delta",
        .method = HTTP_GET,
        .handler = get_led_delta_calculation_handler,
        .user_ctx = NULL
    };

    httpd_register_uri_handler(server_handle, &uri_get_sensors);

    httpd_register_uri_handler(server_handle, &uri_post_fan);
    httpd_register_uri_handler(server_handle, &uri_post_pump);
    httpd_register_uri_handler(server_handle, &uri_post_led);
    httpd_register_uri_handler(server_handle, &uri_post_delta);
};

void networking_init() {
    wifi_init();
    server_init();
}