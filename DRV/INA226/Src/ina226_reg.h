#ifndef _INA226_REG_H_
#define _INA226_REG_H_

/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/* 
 * INA266 register adresses 
 */
#define INA226_REG_CONFIGURATION        (0x00)
#define INA226_REG_SHUNT_VOLTAGE        (0x01)
#define INA226_REG_BUS_VOLTAGE          (0x02)
#define INA226_REG_POWER                (0x03)
#define INA226_REG_CURRENT              (0x04)
#define INA226_REG_CALIBRATION          (0x05)
#define INA226_REG_MASK_ENABLE          (0x06)
#define INA226_REG_ALERT_LIMIT          (0x07)
#define INA226_REG_MANUFACTURER_ID      (0xFE)
#define INA226_REG_DIE_ID               (0xFF)

/*
 * Configuration Register (00h) bit positions
 */
#define INA226_POS_CONFIGURATION_MODE     (0x00)
#define INA226_POS_CONFIGURATION_VSHCT    (0x03)
#define INA226_POS_CONFIGURATION_VBUSCT   (0x06)
#define INA226_POS_CONFIGURATION_AVG      (0x09)
#define INA226_POS_CONFIGURATION_RST      (0x0F)

/*
 * Mask/Enable Register (06h) bit positions
 */
#define INA226_POS_MASK_ENABLE_LEN      (0x00)
#define INA226_POS_MASK_ENABLE_APOL     (0x01)
#define INA226_POS_MASK_ENABLE_OVF      (0x02)
#define INA226_POS_MASK_ENABLE_CVRF     (0x03)
#define INA226_POS_MASK_ENABLE_AFF      (0x04)
#define INA226_POS_MASK_ENABLE_CNVR     (0x0A)
#define INA226_POS_MASK_ENABLE_POL      (0x0B)
#define INA226_POS_MASK_ENABLE_BUL      (0x0C)
#define INA226_POS_MASK_ENABLE_BOL      (0x0D)
#define INA226_POS_MASK_ENABLE_SUL      (0x0E)
#define INA226_POS_MASK_ENABLE_SOL      (0x0F)

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Exported objects ********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************** Exported function prototypes ***************************************
 ***********************************************************************************************************/

#endif  /* _INA226_REG_H_ */
