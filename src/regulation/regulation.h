typedef enum
{
    STARTING,
    MEASURING,
    REGULATE,
    OK
}state;

extern state current_state;
extern int specified_led_strength;

void regulate();

