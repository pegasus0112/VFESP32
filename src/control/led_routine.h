//delta value between red & blue light
/**
 * delta of red blue difference
 * will be set by data from NVS and updated by meassure_led_delta()
 */
extern float DELTA_LEDS_PERCENT;
/**
 * routine for checking strength difference between red and blue LEDs
 * returns sets & delta
*/
float meassure_led_delta();
