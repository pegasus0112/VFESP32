#include <inttypes.h>

void init_nvs_storage();

int32_t read_int_from_storage_by_key(char *key);

float read_float_from_storage_by_key(char *key);

void save_int_value_by_key(char *key, int32_t value);

void save_float_value_by_key(char *key, float value);