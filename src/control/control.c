#include "control.h"
#include "driver/ledc.h"
#include "utils.h"

float FAN_PERCENT;
#define FAN_DUTY (1000)                 //start duty of fan
#define FAN_FREQUENCY (10)              //frequency in Hertz.
#define FAN_TIMER LEDC_TIMER_0
#define FAN_MODE LEDC_LOW_SPEED_MODE
#define FAN_OUTPUT_IO (5)               //output GPIO
#define FAN_CHANNEL LEDC_CHANNEL_0
#define FAN_DUTY_RES LEDC_TIMER_13_BIT  //duty resolution 13 bits

float PUMP_PERCENT = 0;
#define PUMP_DUTY (2700)                //start duty of pump
#define PUMP_FREQUENCY (5000)           //frequency in Hertz.
#define PUMP_TIMER LEDC_TIMER_1
#define PUMP_MODE LEDC_LOW_SPEED_MODE
#define PUMP_OUTPUT_IO (16)             //output GPIO
#define PUMP_CHANNEL LEDC_CHANNEL_1
#define PUMP_DUTY_RES LEDC_TIMER_13_BIT //duty resolution to 13 bits

float LED_PERCENT = 0;
#define LED_DUTY (4095)                 //start duty 50%. ((2 ** 13) - 1) * 50% = 4095
#define LED_FREQUENCY (80)              //frequency in Hertz.
#define LED_TIMER LEDC_TIMER_2
#define LED_MODE LEDC_HIGH_SPEED_MODE
#define LED_OUTPUT_IO (17)              //output GPIO
#define LED_CHANNEL LEDC_CHANNEL_2
#define LED_DUTY_RES LEDC_TIMER_13_BIT  //duty resolution to 13 bits


//initialize pwm (timer & channel) of fans based on FAN_* defines
void init_fan()
{
    //prepare, then apply FAN PWM timer configuration
    ledc_timer_config_t fan_timer = {
        .speed_mode = FAN_MODE,
        .duty_resolution = FAN_DUTY_RES,
        .timer_num = FAN_TIMER,
        .freq_hz = FAN_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&fan_timer));

    // Prepare,then apply FAN PWM channel configuration
    ledc_channel_config_t fan_channel = {
        .gpio_num = FAN_OUTPUT_IO,
        .speed_mode = FAN_MODE,
        .channel = FAN_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = FAN_TIMER,
        .duty = FAN_DUTY,
        .hpoint = 0
        };
    ESP_ERROR_CHECK(ledc_channel_config(&fan_channel));

    FAN_PERCENT = remap_float_to_range(FAN_DUTY, 1, 100, FAN_MIN, FAN_MAX);
}

//initialize pwm (timer & channel) of pumps based on PUMP_* defines
void init_pump()
{
    //prepare, then apply PUMP PWM timer configuration
    ledc_timer_config_t PUMP_timer = {
        .speed_mode = PUMP_MODE,
        .duty_resolution = PUMP_DUTY_RES,
        .timer_num = PUMP_TIMER,
        .freq_hz = PUMP_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&PUMP_timer));

// Prepare,then apply PUMP PWM channel configuration
    ledc_channel_config_t PUMP_channel = {
        .gpio_num = PUMP_OUTPUT_IO,
        .speed_mode = PUMP_MODE,
        .channel = PUMP_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = PUMP_TIMER,
        .duty = PUMP_DUTY,
        .hpoint = 0
        };
    ESP_ERROR_CHECK(ledc_channel_config(&PUMP_channel));

    PUMP_PERCENT = remap_float_to_range(PUMP_DUTY, 1, 100, PUMP_MIN, PUMP_MAX);
}

//initialize pwm (timer & channel) of LEDs based on LED_* defines
void init_led()
{
    //prepare, then apply LED PWM timer configuration
    ledc_timer_config_t LED_timer = {
        .speed_mode = LED_MODE,
        .duty_resolution = LED_DUTY_RES,
        .timer_num = LED_TIMER,
        .freq_hz = LED_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&LED_timer));

    // Prepare,then apply LED PWM channel configuration
    ledc_channel_config_t LED_channel = {
        .gpio_num = LED_OUTPUT_IO,
        .speed_mode = LED_MODE,
        .channel = LED_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LED_TIMER,
        .duty = LED_DUTY,
        .hpoint = 0
        };
    ESP_ERROR_CHECK(ledc_channel_config(&LED_channel));

    LED_PERCENT = remap_float_to_range(LED_DUTY, 1, 100, LED_MIN, LED_MAX);
}

// initialize & start pwm for fans, pumps & LEDs
void pwm_init(void)
{
    //initialize & start fan pwm
    init_fan();
    //initialize & start pump pwm
    init_pump();
    //initialize & start led pwm
    init_led();
}

//change fan duty and update pwm channel
void update_duty_fan(int duty)
{
    // change duty settings of fan
    ledc_set_duty(FAN_MODE, FAN_CHANNEL, duty);
    // update duty to apply the new value
    ledc_update_duty(FAN_MODE, FAN_CHANNEL);
}

//change pump duty and update pwm channel
void update_duty_pump(int duty)
{
    // change duty settings of fan
    ledc_set_duty(PUMP_MODE, PUMP_CHANNEL, duty);
    // update duty to apply the new value
    ledc_update_duty(PUMP_MODE, PUMP_CHANNEL);
}

//change led duty and update pwm channel
void update_duty_led(int duty)
{
    // change duty settings of fan
    ledc_set_duty(LED_MODE, LED_CHANNEL, duty);
    // update duty to apply the new value
    ledc_update_duty(LED_MODE, LED_CHANNEL);
}

//parameter is percentual power of fan
//return string of methode executed state
char * change_duty_fan(int duty) 
{
    if (duty < 0)
    {
        return "set fan duty not between 0-100";
    }
    
    if (duty > 100)
    {
        FAN_PERCENT = 100;
        update_duty_fan(FAN_MAX);
        return "set fan duty to max";
    }

    if (duty == 0)
    {
        FAN_PERCENT = 0;

        update_duty_fan(0);
        return "fans off";
        } else
        {
            FAN_PERCENT= remap_float_to_range(duty, 1, 100, FAN_MIN, FAN_MAX);            
            update_duty_fan(FAN_PERCENT);
            return "fan duty updated";
    }
}

//parameter is percentual power of pump
//return string of methode executed state
char * change_duty_pump(int duty) 
{
    if (duty > 100 || duty < 0)
    {
        return "set pump duty not between 0-100";
        }

    if (duty == 0)
    {
        PUMP_PERCENT = 0;

        update_duty_pump(0);
        return "pump off";
        } else
        {
            PUMP_PERCENT = remap_float_to_range(duty, 1, 100, PUMP_MIN, PUMP_MAX);            
            update_duty_pump(PUMP_PERCENT);
            return "pump duty updated";
    }
}

//parameter is percentual power of led
//return string of methode executed state
char * change_duty_led(int duty) 
{
    if (duty > 100 || duty < 0)
    {
        return "set led duty not between 0-100";
        }

    if (duty == 0)
    {
        LED_PERCENT = 0;

        update_duty_led(0);
        return "leds off";
        } else
        {
            LED_PERCENT = remap_float_to_range(duty, 1, 100, LED_MIN, LED_MAX);
            update_duty_fan(LED_PERCENT);
            return "led duty updated";
    }
}