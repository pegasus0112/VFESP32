#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO          22               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO          21               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ         400000           /*!< I2C master clock frequency */

#define BH1750_SENSOR_ADDR         0x23             /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START           0x10             /*!< Command to set measure mode */
#define WRITE_BIT                  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN               0x1              /*!< I2C master will check ack from slave*/
#define ACK_VAL                    0x0              /*!< I2C ack value */
#define NACK_VAL                   0x1              /*!< I2C nack value */

#define portTICK_RATE_MS 10000

/**
 * reading bh1750 i2c sensor address
*/
static esp_err_t read_bh1750(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * initializing bh1750 to high resolution continuous mode
 * nased on defines
*/
static void init_bh1750(i2c_port_t i2c_num) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, BH1750_CMD_START, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

/**
 * initializing i2c master & bh1750 based on define values
 */
static void i2c_master_init()
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_DISABLE,
                       I2C_MASTER_TX_BUF_DISABLE, 0);
    init_bh1750(I2C_MASTER_NUM);
}

/**
 * reading brightness value from bh1750 ic2 sensor
 * returns read birghtness value
*/
static float get_brightness()
{
    int ret;
    float value = 0;
    uint8_t sensor_data_h, sensor_data_l;
    while (1) {
        ret = read_bh1750( I2C_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if(ret == ESP_ERR_TIMEOUT) {
            printf("I2C timeout\n");
        } else if(ret == ESP_OK) {
            value = (sensor_data_h << 8 | sensor_data_l) / 1.2;
        } else {
            printf("No ack, sensor not connected...skip...\n");
        }
        return value;
    }
}