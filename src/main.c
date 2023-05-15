#include "utils.h"
#include "controle/controle.h"
#include "sensors/sensors.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    pwm_init();
    init_sensors();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        read_allSensor_Data();
        float bright = BRIGHTNESS;
        printf("brightness: %f \n", bright);
        bright = remap_float_to_range(bright, 0,400,0,100);
        printf("remapped brightness: %f \n", bright);

        change_duty_fan(bright);
    }
}