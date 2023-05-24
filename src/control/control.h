extern float FAN_PERCENT;
#define FAN_MIN (900)                   //Minumum fan duty
#define FAN_MAX (8192)                  //Maximum fan duty

extern float PUMP_PERCENT;
#define PUMP_MIN (2700)                  //Minumum pump duty
#define PUMP_MAX (8192)                  //Maximum pump duty

extern float LED_RED_PERCENT;
extern float LED_BLUE_PERCENT;
#define LED_MIN (1000)                  //Minumum led duty
#define LED_MAX (8192)                  //Maximum led duty

void pwm_init(void);


extern float blue_proportion_percent;

char * change_duty_fan(int duty);
char * change_duty_pump(int duty);
char * change_duty_led_red(int duty);
char * change_duty_led_blue(int duty);
void update_proportion_led_blue_to_red();




