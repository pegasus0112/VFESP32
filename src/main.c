#include "regulation/regulation.h"
#include "control/control.h"
#include "nvs/nvs_handler.h"

#include "esp_timer.h"
#include "esp_err.h"
#include "nvs_flash.h"

void app_main()
{
    init_nvs_storage();

    init_timer_fcts();

    //creating timer for periodically calling regulate()
    const esp_timer_create_args_t reg_timer_args = {
        .callback = &regulate,
        .name = "regulation timer"
    };


    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&reg_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 3000000));
    
}