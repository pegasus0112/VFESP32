#include "controle.h"
#include "driver/ledc.h"

static float Remap(float value, float from1, float to1, float from2, float to2) {
    return (value - from1)/(to1 - from1)*(to2 - from2)+from2;
}

void ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = FAN_MODE,
        .timer_num        = FAN_TIMER,
        .duty_resolution  = FAN_DUTY_RES,
        .freq_hz          = FAN_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = FAN_MODE,
        .channel        = FAN_CHANNEL,
        .timer_sel      = FAN_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void pwm_init(void)
{
    // Set the LEDC peripheral configuration
    ledc_init();
    // Set duty of Fans to const FAN_DUTY
    ledc_set_duty(FAN_MODE, FAN_CHANNEL, FAN_MIN);
    // Update duty to apply the new value
    ledc_update_duty(FAN_MODE, FAN_CHANNEL);
}

void updateDutyMotor(int duty) {
    ledc_set_duty(FAN_MODE, FAN_CHANNEL, duty);
    // Update duty to apply the new value
    ledc_update_duty(FAN_MODE, FAN_CHANNEL);
}

/*return string if operation is successful*/
char * changeDutyMotor(int duty) {

    printf("new Value: %d \n", duty);

    if (duty > 100 || duty < 0)
    {
        return "set duty not between 0-100";
    }

    if (duty == 0) {
        updateDutyMotor(0);
        return "Motor off";
    } else {
        float MAPPED_VALUE = Remap(duty, 1, 100, FAN_MIN, FAN_MAX);

        printf("mapped duty: %f \n", MAPPED_VALUE);

        updateDutyMotor(MAPPED_VALUE);
        return "updated duty";
        }
}