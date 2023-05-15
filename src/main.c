#include "regulation/regulation.h"
#include "control/control.h"
#include "sensors/sensors.h"

#include "esp_timer.h"
#include "esp_err.h"

void timer_isr_regulation() {
    regulate();
}

void app_main()
{
    pwm_init();
    init_sensors();
    const esp_timer_create_args_t reg_timer_args = {
        .callback = &timer_isr_regulation,
        .name = "regulation timer"};
      
    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&reg_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 1000000));
}