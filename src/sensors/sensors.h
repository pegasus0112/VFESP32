extern float BRIGHTNESS;
extern int TEMPERATURE;
extern int HUMIDITY;
//extern int HUMIDITY_SUM;
//extern int AVERAGE_HUMIDITY;
extern int WATER_DISTANCE;

//Timer for measurements over a period of time
//extern int timer_measurement = 0;

// initialze sensors so sensors can read data
void init_sensors();

// reading all sensors & update attributes with new values
void read_allSensor_Data();