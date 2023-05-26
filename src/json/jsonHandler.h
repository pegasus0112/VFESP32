/**
 * parsing int from json_string root (not children of children)
 * to parse int specified by key of json value
*/
int get_int_from_Json(char* json_string, char* key);

/**
 * converting all values from sensors and actuators into json
 * return new created json
*/
char* get_farm_status_as_json_string();

/**
 * parsing string from json_string root (not children of children) by key
*/
char * get_string_from_Json(char* json_string, char* key);