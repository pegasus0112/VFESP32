#include <stdio.h>
#include "utils.h"

#include "regulation.h"
#include "control/control.h"
#include "sensors/sensors.h"

state current_state = STARTING;

void regulate_fan_based_on_light()
{
    printf("brightness: %f \n", BRIGHTNESS);
    change_duty_fan(remap_float_to_range(BRIGHTNESS, 0, 1000, 0, 100));
};

void regulate_fan_based_on_temperature()
{
    // TODO
};

void regulate_refill_pump_based_on_ultrasonic()
{
    // TODO
};

void regulate()
{
    switch (current_state)
    {
        case STARTING:
            break;
        case MEASURING:
            break;
        case OK:
            read_allSensor_Data();
            regulate_fan_based_on_light();
            break;
    }
};