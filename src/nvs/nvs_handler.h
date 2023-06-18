#include <inttypes.h>

/**
 * initializing nvs and checks for errors
 * if it is full or has been changed externally, storage will be cleared
*/
void init_nvs_storage();

/**
 * reads int32 value from nvs storage by key
 * returns int32 value from storage
*/
int32_t read_int_from_storage_by_key(char *key);

/**
 * reads float value from nvs storage by key
 * returns float value from storage
 * important: float is read as string
*/
float read_float_from_storage_by_key(char *key);

/**
 * saves int value in nvs storage by key
*/
void save_int_value_by_key(char *key, int32_t value);

/**
 * saves float value in nvs storage by key
 * important: float will be converted to char
 * only last two digits will be safed
*/
void save_float_value_by_key(char *key, float value);

/**
 * reads all used by system values from storage and set them
 * example: reads old led_delta value and sets it in led_routine
*/
void read_all_saved_data_from_nvs();