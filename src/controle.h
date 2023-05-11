#define LEDC_OUTPUT_IO          (5) // Define the output GPIO

#define FAN_FREQUENCY          (10) // Frequency in Hertz. Set frequency at 5 kHz

#define FAN_TIMER              LEDC_TIMER_0
#define FAN_MODE               LEDC_LOW_SPEED_MODE
#define FAN_CHANNEL            LEDC_CHANNEL_0
#define FAN_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits

#define FAN_MIN                (900)
#define FAN_MAX                (8192)

void ledc_init(void);

void pwm_init(void);

void updateDutyMotor(int duty);

char * changeDutyMotor(int duty);