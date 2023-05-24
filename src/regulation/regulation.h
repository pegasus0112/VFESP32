typedef enum
{
    STARTING,
    MEASURING,
    OK
}state;

extern state current_state;

void regulate();

