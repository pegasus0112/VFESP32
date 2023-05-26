extern float BRIGHTNESS;
extern float TEMPERATURE;
extern float HUMIDITY;
extern int WATER_DISTANCE;

// initialze sensors so sensors can read data
void init_sensors();

// reading all sensors & update attributes with new values
void read_allSensor_Data();