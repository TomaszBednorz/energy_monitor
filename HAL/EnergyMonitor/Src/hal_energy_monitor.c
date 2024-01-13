/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include "hal_energy_monitor.h"
#include "hal_energy_monitor_cfg.h"
#include "ina226.h"
#include "cmsis_os.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/
/*
 * Possible states of module
 */
typedef enum
{
    EnergyMonitor_StateUninit = 0,
    EnergyMonitor_StateBusVoltage,
    EnergyMonitor_StatePower,
    EnergyMonitor_StateCurrent,
    EnergyMonitor_StateFinished
}Hal_EnergyMonitor_State_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

static void Hal_EnergyMonitor_Task(void const * argument);
static void Hal_EnergyMonitor_ReadResults(void);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

static Hal_EnergyMonitor_Data_t Hal_EnergyMonitor_Data;

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

osThreadId Hal_EnergyMonitorTaskHandle;
Hal_EnergyMonitor_State_t Hal_EnergyMonitor_State = EnergyMonitor_StateUninit;

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

/*!	
 * \brief HAL energy monitor initialization function
 *
 * \param[in] None
 * 
 * \retval None
 */
void Hal_EnergyMonitor_Init(void)
{
    /* Create thread */
    osThreadDef(EnergyMonitor_Task, Hal_EnergyMonitor_Task, osPriorityNormal, 0, 128);
    Hal_EnergyMonitorTaskHandle = osThreadCreate(osThread(EnergyMonitor_Task), NULL);

    Hal_EnergyMonitor_State = EnergyMonitor_StateBusVoltage;
}

/*!	
 * \brief Get results
 *
 * \param[in] data Pointer to store results
 * 
 * \retval None
 */
void Hal_EnergyMonitor_GetResults(Hal_EnergyMonitor_Data_t* data)
{
    data->bus_voltage = Hal_EnergyMonitor_Data.bus_voltage;
    data->current = Hal_EnergyMonitor_Data.current;
    data->power = Hal_EnergyMonitor_Data.power;
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/

/*!	
 * \brief HAL energy monitor task
 *
 * \param[in] argument OS required parameter
 * 
 * \retval None
 */
static void Hal_EnergyMonitor_Task(void const * argument)
{
    uint32_t time = osKernelSysTick();

    while(1)
    {
        switch (Hal_EnergyMonitor_State)
        {
            case EnergyMonitor_StateBusVoltage:
                INA226_ReadMeasurement(INA226_BusVoltage);
                Hal_EnergyMonitor_State++;
                osDelayUntil(&time, 1U);
                break;
            case EnergyMonitor_StatePower:
                INA226_ReadMeasurement(INA226_Power);
                Hal_EnergyMonitor_State++;
                osDelayUntil(&time, 1U);
                break;
            case EnergyMonitor_StateCurrent:
                INA226_ReadMeasurement(INA226_Current);
                Hal_EnergyMonitor_State++;
                osDelayUntil(&time, 1U);
                break;
            case EnergyMonitor_StateFinished:
                Hal_EnergyMonitor_ReadResults();
                Hal_EnergyMonitor_State = EnergyMonitor_StateBusVoltage;
                osDelayUntil(&time, 47U);  /* 50ms = 47+1+1+1, measurements are read every 50ms */
                break;
            default:
                /* Do nothing */
                break;
        }
    }
}

/*!	
 * \brief Function reads results from the lower layer
 *
 * \param[in] None
 * 
 * \retval None
 */
static void Hal_EnergyMonitor_ReadResults(void)
{
    uint16_t result = 0U;

    /* Read bus voltage */
    result = INA226_GetResult(INA226_BusVoltage);
    Hal_EnergyMonitor_Data.bus_voltage = result * HAL_ENERGY_MONITOR_BUS_VOLTAGE_LSB;

    /* Read power */
    result = INA226_GetResult(INA226_Power);
    Hal_EnergyMonitor_Data.power = result * HAL_ENERGY_MONITOR_POWER_LSB * 1000.0f;

    /* Read current */
    result = INA226_GetResult(INA226_Current);
    Hal_EnergyMonitor_Data.current = result * HAL_ENERGY_MONITOR_CURRENT_LSB * 1000.0f;
}
