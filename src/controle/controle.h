extern float FAN_PERCENT;
#define FAN_MIN (900)                   //Minumum fan duty
#define FAN_MAX (8192)                  //Maximum fan duty

extern float PUMP_PERCENT;
#define PUMP_MIN (2700)                  //Minumum pump duty
#define PUMP_MAX (8192)                  //Maximum pump duty

extern float LED_PERCENT;
#define LED_MIN (2700)                  //Minumum led duty
#define LED_MAX (8192)                  //Maximum led duty

void pwm_init(void);

char * change_duty_fan(int duty);
char * change_duty_pump(int duty);
char * change_duty_led(int duty);



