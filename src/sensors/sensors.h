extern float BRIGHTNESS;
extern int TEMPERATURE;
extern int HUMIDITY;
extern int WATER_DISTANCE;

// initialze sensors so sensors can read data
void init_sensors();

// reading all sensors & update attributes with new values
void read_allSensor_Data();