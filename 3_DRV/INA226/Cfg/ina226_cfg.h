#ifndef _INA226_CFG_H_
#define _INA226_CFG_H_

/* 
 * INA226 configuration file - all below defines should be filled by the user
 */

/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include "i2c.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

#define INA226_I2C_ADDR                     (0x40)

/*!	
 * \brief I2C receive function
 *
 * \param[in] buf_ptr Buffer pointer for reception data
 * \param[in] size Size of data to receive
 * 
 * \retval None
 */
#define INA226_Receive(buf_ptr, size)       (HAL_I2C_Master_Receive_IT(&hi2c1, (INA226_I2C_ADDR << 1U), buf_ptr, size))

/*!	
 * \brief I2C transmit function
 *
 * \param[in] buf_ptr Buffer pointer for transmission data
 * \param[in] size Size of data to transmit
 * 
 * \retval None
 */
#define INA226_Transmit(buf_ptr, size)      (HAL_I2C_Master_Transmit_IT(&hi2c1, (INA226_I2C_ADDR << 1U), buf_ptr, size))

/*
 * Status codes
 */
#define INA226_CODE_OK                  (0U)
#define INA226_CODE_NOT_OK              (1U)

/*
 * Configuration according to INA226 datasheet
 */

/* Configuration Register (00h) */
#define INA226_CFG_CONFIGURATION_MODE       (0x07)  /* Operating Mode - Shunt and Bus, Continuous */
#define INA226_CFG_CONFIGURATION_VSHCT      (0x07)  /* Shunt Voltage Conversion Time - 8.244 ms */
#define INA226_CFG_CONFIGURATION_VBUSCT     (0x07)  /* Bus Voltage Conversion Time - 8.244 ms */
#define INA226_CFG_CONFIGURATION_AVG        (0x01)  /* Averaging Mode - 4 */
#define INA226_CFG_CONFIGURATION_RST        (0x00)  /* Reset Bit - 0 */

/* Calibration Register (05h)  
 *
 * Max expected current (MEC) - 1.024A 
 * Shunt register (SR) - 0.1 Ohm 
 * CAL = 0.00512 /((MEC / 2^15) * SR) = 16 38.4 (0x0666)
 */
#define INA226_CFG_CALIBRATION              (0x0666)

/* Mask/Enable Register (06h) */
#define INA226_CFG_MASK_ENABLE_LEN      (0x00)  /* Transparent (default) */
#define INA226_CFG_MASK_ENABLE_APOL     (0x00)  /* Normal (active-low open collector) (default) */

#define INA226_CFG_MASK_ENABLE_CNVR     (0x00)  /* Conversion Ready - DISABLED*/
#define INA226_CFG_MASK_ENABLE_POL      (0x01)  /* Power Over-Limit - ENABLED */
#define INA226_CFG_MASK_ENABLE_BUL      (0x00)  /* Bus Voltage Under-Voltage - DISABLED */
#define INA226_CFG_MASK_ENABLE_BOL      (0x00)  /* Bus Voltage Over-Voltage - DISABLED */
#define INA226_CFG_MASK_ENABLE_SUL      (0x00)  /* Shunt Voltage Under-Voltage - DISABLED */
#define INA226_CFG_MASK_ENABLE_SOL      (0x00)  /* Shunt Voltage Over-Voltage - DISABLED */

/* Alert Limit Register (07h) */
#define INA226_CFG_ALERT_LIMIT              (0x0066) // 80mW - Power Over-Limit threshold 

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Exported objects ********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************** Exported function prototypes ***************************************
 ***********************************************************************************************************/

#endif  /* _INA226_CFG_H_ */
