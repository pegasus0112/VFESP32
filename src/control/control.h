extern float FAN_PERCENT;
#define FAN_MIN (900)                   //Minumum fan duty
#define FAN_MAX (8192)                  //Maximum fan duty

extern float PUMP_PERCENT;
#define PUMP_MIN (2700)                  //Minumum pump duty
#define PUMP_MAX (8192)                  //Maximum pump duty

extern float LED_RED_PERCENT;
extern float LED_BLUE_PERCENT;
#define LED_MIN (100)                  //Minumum led duty
#define LED_MAX (8192)                  //Maximum led duty

extern bool PUMP_REFILL_ON;

void pwm_init(void);


extern float blue_proportion_percent;
/**
 * parameter is percentual power of fan
 * return string of methode executed state
*/
char * change_duty_fan(int duty);

/**
 * parameter is percentual power of pump 
 * return string of methode executed state
 */
char * change_duty_pump(int duty);

/**
 * parameter is percentual power of red led
 * return string of methode executed state
*/
char * change_duty_led_red(int duty);

/**
 * parameter is percentual power of blue led
 * return string of methode executed state
*/
char * change_duty_led_blue(int duty);

/**
 * enabling or disabling refill pump, based on bool
 * true == on
 */
void set_state_pump_refill(bool pump_enabled);


