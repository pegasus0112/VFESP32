/**
 * enum containing all states of regulate status
 * STARTING: at startup
 * MEASURING: measuring values IMPORTAINT is. that measuring can influence actuators
 * OK: running state where sensors will be refreshed & actuators controle 
*/
typedef enum
{
    STARTING,
    MEASURING,
    OK
}state;

extern state current_state;
extern int specified_led_strength;

/**
 * regulation methode taht will be called timer periodically
 * period decided by main (current every 1s)
*/
void regulate();

