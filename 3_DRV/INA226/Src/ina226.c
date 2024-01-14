/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include "ina226_reg.h"
#include "ina226_cfg.h"
#include "ina226.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

#define INA226_ADDR_SIZE (1U)
#define INA226_REG_SIZE  (2U)
#define INA226_BUF_SIZE  (3U)

/*!	
 * \brief Get most significant byte
 *
 * \param[in] data_16b 16-bit data
 * 
 * \retval MS byte of 16-bit data
 */
#define INA226_GetMSByte(data_16b)              ((uint8_t)(((data_16b) & 0xFF00) >> 8U))

/*!	
 * \brief Get least significant byte
 *
 * \param[in] data_16b 16-bit data
 * 
 * \retval LS byte of 16-bit data
 */
#define INA226_GetLSByte(data_16b)              ((uint8_t)((data_16b) & 0x00FF))

/*!	
 * \brief Concatenate bytes into 16-bit data
 *
 * \param[in] data_ptr pointer to two-byte buffer
 * 
 * \retval Concatenated data
 */
#define INA226_ConcatenateBytes(data_ptr)       ((uint16_t)(((uint16_t)data_ptr[0] << 8U) | data_ptr[1]))

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/*
 * Driver status
 */
typedef enum
{
    INA226_Uninitialized = 0,
    INA226_Ready,
    INA226_BusyTx,
    INA226_BusyRx
}INA226_Status_t;

/*
 * Transfer data
 */
typedef union 
{
    uint8_t buf[INA226_BUF_SIZE];
    struct ina226
    {
        uint8_t reg_Addr;
        uint8_t data[INA226_REG_SIZE];
    }field;
}INA226_Transfer_t;

/*
 * Results data
 */
typedef struct 
{
    uint16_t shunt_voltage;
    uint16_t bus_voltage;
    uint16_t power;
    uint16_t current;
}INA226_Results_t;

/*
 * All device information
 */
typedef struct 
{
    volatile INA226_Status_t status;
    INA226_Transfer_t transfer;
    INA226_Results_t results;
}INA226_Device_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

static uint8_t INA226_Write(INA226_Device_t* dev);
static uint8_t INA226_Read(INA226_Device_t* dev);
static void INA226_CollectResult(void);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static INA226_Device_t INA226_Device;

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

/*!	
 * \brief INA226 initialization function
 *
 * \param[in] None
 * 
 * \retval None
 */
void INA226_Init(void)
{
    uint16_t tx_data = 0U;
    uint8_t status = INA226_CODE_OK;

    INA226_Device.status = INA226_Ready;

    /* Configuration Register */
    tx_data = ((INA226_CFG_CONFIGURATION_MODE << INA226_POS_CONFIGURATION_MODE) | \
               (INA226_CFG_CONFIGURATION_VSHCT << INA226_POS_CONFIGURATION_VSHCT)  | \
               (INA226_CFG_CONFIGURATION_VBUSCT << INA226_POS_CONFIGURATION_VBUSCT)  | \
               (INA226_CFG_CONFIGURATION_AVG << INA226_POS_CONFIGURATION_AVG)  | \
               (INA226_CFG_CONFIGURATION_RST << INA226_POS_CONFIGURATION_RST));
       
    INA226_Device.transfer.field.data[0] = INA226_GetMSByte(tx_data);
    INA226_Device.transfer.field.data[1] = INA226_GetLSByte(tx_data);
    INA226_Device.transfer.field.reg_Addr = INA226_REG_CONFIGURATION;

    status = INA226_Write(&INA226_Device);

    /* Wait for transmision complete */
    while(INA226_Device.status != INA226_Ready);

    /* Calibration Register (05h) */
    tx_data = INA226_CFG_CALIBRATION;
    INA226_Device.transfer.field.data[0]  = INA226_GetMSByte(tx_data);
    INA226_Device.transfer.field.data[1] = INA226_GetLSByte(tx_data);
    INA226_Device.transfer.field.reg_Addr = INA226_REG_CALIBRATION;

    if(status == INA226_CODE_OK)
    {
        INA226_Write(&INA226_Device);
    }
    
    /* Wait for transmision complete */
    while(INA226_Device.status != INA226_Ready);

    /* Mask/Enable Register (06h) */
    tx_data = ((INA226_CFG_MASK_ENABLE_LEN << INA226_POS_MASK_ENABLE_LEN) | \
               (INA226_CFG_MASK_ENABLE_APOL << INA226_POS_MASK_ENABLE_APOL) | \
               (INA226_CFG_MASK_ENABLE_CNVR << INA226_POS_MASK_ENABLE_CNVR) | \
               (INA226_CFG_MASK_ENABLE_POL << INA226_POS_MASK_ENABLE_POL) | \
               (INA226_CFG_MASK_ENABLE_BUL << INA226_POS_MASK_ENABLE_BUL) | \
               (INA226_CFG_MASK_ENABLE_BOL << INA226_POS_MASK_ENABLE_BOL) | \
               (INA226_CFG_MASK_ENABLE_SUL << INA226_POS_MASK_ENABLE_SUL) | \
               (INA226_CFG_MASK_ENABLE_SOL << INA226_POS_MASK_ENABLE_SOL));
    INA226_Device.transfer.field.data[0] = INA226_GetMSByte(tx_data);
    INA226_Device.transfer.field.data[1] = INA226_GetLSByte(tx_data);
    INA226_Device.transfer.field.reg_Addr = INA226_REG_MASK_ENABLE;

    if(status == INA226_CODE_OK)
    {
        INA226_Write(&INA226_Device);
    }

    /* Wait for transmision complete */
    while(INA226_Device.status != INA226_Ready);

    /* Alert Limit Register (07h) */
    tx_data = INA226_CFG_ALERT_LIMIT;
    INA226_Device.transfer.field.data[0] = INA226_GetMSByte(tx_data);
    INA226_Device.transfer.field.data[1] = INA226_GetLSByte(tx_data);
    INA226_Device.transfer.field.reg_Addr = INA226_REG_ALERT_LIMIT;

    if(status == INA226_CODE_OK)
    {
        INA226_Write(&INA226_Device);
    }

    /* Wait for transmision complete */
    while(INA226_Device.status != INA226_Ready);

}

/*!	
 * \brief INA226 start measurement function
 *
 * \param[in] data_type Data type to receive
 * 
 * \retval Status code
 */
uint8_t INA226_ReadMeasurement(INAA226_DataType_t data_type)
{
    uint8_t ret_val;

    switch (data_type)
    {
        case INA226_ShuntVoltage:
            INA226_Device.transfer.field.reg_Addr = INA226_REG_SHUNT_VOLTAGE;
            ret_val = INA226_Read(&INA226_Device);
            break;
        case INA226_BusVoltage:
            INA226_Device.transfer.field.reg_Addr = INA226_REG_BUS_VOLTAGE;
            ret_val = INA226_Read(&INA226_Device);
            break;
        case INA226_Power:
            INA226_Device.transfer.field.reg_Addr = INA226_REG_POWER;
            ret_val = INA226_Read(&INA226_Device);
            break;
        case INA226_Current:
            INA226_Device.transfer.field.reg_Addr = INA226_REG_CURRENT;
            ret_val = INA226_Read(&INA226_Device);
            break;
        default:
            break;
    }

    return ret_val;
}

/*!	
 * \brief Get result
 *
 * \param[in] data_type Data type to return
 * 
 * \retval Result based on the data_type parameter.
 */
uint16_t INA226_GetResult(INAA226_DataType_t data_type)
{
    uint16_t ret_val = 0U;

    switch (data_type)
    {
        case INA226_ShuntVoltage:
            ret_val = INA226_Device.results.shunt_voltage;
            break;
        case INA226_BusVoltage:
            ret_val = INA226_Device.results.bus_voltage;
            break;
        case INA226_Power:
            ret_val = INA226_Device.results.power;
            break;
        case INA226_Current:
            ret_val = INA226_Device.results.current;
            break;
        default:
            break;
    }

    return ret_val;
}

/*!	
 * \brief I2C read complete callback - should be called from ISR
 *
 * \param[in] None
 * 
 * \retval None
 */
void INA226_ReadCompleteCb(void)
{
    if(INA226_Device.status == INA226_BusyRx)
    {
        INA226_Device.status = INA226_Ready;
        INA226_CollectResult();
    }
}

/*!	
 * \brief I2C write complete callback - should be called from ISR
 *
 * \param[in] None
 * 
 * \retval None
 */
void INA226_WriteCompleteCb(void)
{
    if(INA226_Device.status == INA226_BusyTx)
    {
        INA226_Device.status = INA226_Ready;
    }
    else if(INA226_Device.status == INA226_BusyRx)
    {
        INA226_Receive(INA226_Device.transfer.field.data, INA226_REG_SIZE);
    }
    else
    {
        /* Do nothing */
    }
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/

/*!	
 * \brief INA226 write function
 *
 * \param[in] dev Device information object pointer
 * 
 * \retval Status code
 */
static uint8_t INA226_Write(INA226_Device_t* dev)
{
    uint8_t ret_val = INA226_CODE_OK;

    if(dev->status != INA226_Ready)
    {
        ret_val = INA226_CODE_NOT_OK;  /* Invalid transmission */
    }
    else
    {
        dev->status = INA226_BusyTx;
        INA226_Transmit(dev->transfer.buf, INA226_BUF_SIZE);
    }

    return ret_val;
}

/*!	
 * \brief INA226 read function
 *
 * \param[in] dev Device information object pointer
 * 
 * \retval Status code
 */
static uint8_t INA226_Read(INA226_Device_t* dev)
{
    uint8_t ret_val = INA226_CODE_OK;

    if(dev->status != INA226_Ready)
    {
        ret_val = INA226_CODE_NOT_OK;  /* Invalid reception */
    }
    else
    {
        dev->status = INA226_BusyRx;
        INA226_Transmit(&(dev->transfer.field.reg_Addr), INA226_ADDR_SIZE);
    }

    return ret_val;
}

/*!	
 * \brief Function collects received data
 *
 * \param[in] None
 * 
 * \retval None
 */
static void INA226_CollectResult(void)
{
    switch (INA226_Device.transfer.field.reg_Addr)
    {
        case INA226_REG_SHUNT_VOLTAGE:
            INA226_Device.results.shunt_voltage = INA226_ConcatenateBytes(INA226_Device.transfer.field.data);
            break;
        case INA226_REG_BUS_VOLTAGE:
            INA226_Device.results.bus_voltage = INA226_ConcatenateBytes(INA226_Device.transfer.field.data);
            break;
        case INA226_REG_POWER:
            INA226_Device.results.power = INA226_ConcatenateBytes(INA226_Device.transfer.field.data);
            break;
        case INA226_REG_CURRENT:
            INA226_Device.results.current = INA226_ConcatenateBytes(INA226_Device.transfer.field.data);
            break;
        default:
            break;
    }
}
