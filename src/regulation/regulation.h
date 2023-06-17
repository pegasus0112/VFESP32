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

/**
 * specifies current state of regulation
*/
extern state current_state;

/**
 * specified led strength of user
 * set via NVS or http POST Rquest
 */
extern int specified_led_strength;


/**
 * specified temperature of user
 * set via NVS or http POST Rquest
 * optimum temperature for parsley is 22 degrees
 * max range between 10-27 degrees
 */
extern int specified_temperature;

/**
 * specified humidity of user
 * set via NVS or http POST Rquest
 * optimum humidity for parsley is between 40-60%
 */
extern int specified_humidity;

/**
 * minumum water level of water cycle in farm
 * if reached refill pump starts
*/
extern int min_water_level;

/**
 * maximum water level of water cycle in farm
 * if reached refill pump stops
*/
extern int max_water_level;

/**
 * regulation methode taht will be called timer periodically
 * period decided by main (current every 1s)
*/
void regulate();

