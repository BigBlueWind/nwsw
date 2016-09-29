#ifndef LTE_BB_TEMP_H
#define LTE_BB_TEMP_H

#define I2C_BUS_DEV     "/dev/i2c-0"
#define I2C_SLAVE       0x0703       /* Change slave address                 */
                                     /* Attn.: Slave address is 7 or 10 bits */
#define I2C_SLAVE_FORCE 0x0706       /* Change slave address                 */
                                     /* Attn.: Slave address is 7 or 10 bits */
                                     /* This changes the address, even if it */
                                     /* is already taken!                    */
#define I2C_TENBIT      0x0704       /* 0 for 7 bit addrs, != 0 for 10 bit   */

#define I2C_ADDR_LTE_TEMP_SENSOR   0x4A
#define LTE_TEMP_CONF_REG_ADDR     0x1
#define LTE_TEMP_REG_ADDR          0x0
#define LTE_TEMP_CONF_DATA         0x60 // BIT 6 and BIT 7
#define LTE_TEMP_PRECISION_FACTOR  0.0625

typedef enum e_TempSensorReturnCode
{
    TEMPSENSORRETURNCODE_ERR_I2C_DRV_OPEN = 30,
    TEMPSENSORRETURNCODE_STATUS_SUCCESS,
    TEMPSENSORRETURNCODE_STATUS_FAILURE,

}TempSensorReturnCode;

/* extern functions */
extern TempSensorReturnCode readTempSensor(float *temperature);
extern TempSensorReturnCode initTempSensor (void);

#endif /* LTE_PLTM_TEMP_H */

