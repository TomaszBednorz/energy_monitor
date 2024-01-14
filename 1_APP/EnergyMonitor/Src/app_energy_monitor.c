/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <string.h>
#include <stdio.h>
#include "app_energy_monitor.h"
#include "app_energy_monitor_cfg.h"
#include "hal_energy_monitor.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "cmsis_os.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

#define APP_ENERGY_MONITOR_TICKS_IN_S       (1000U)     /* 1s = 1000 ticks */
#define APP_ENERGY_MONITOR_S_IN_MIN         (60U)     /* 1min = 60s */
#define APP_ENERGY_MONITOR_MIN_IN_H         (60U)     /* 1h = 60min */

#define APP_ENERGY_MONITOR_LOG_LEN          (90U)

/*!	
 * \brief Macro converts global time into hours
 *
 * \param[in] time Global time in miliseconds
 * 
 * \retval Number of hours elapsed since system start-up
 */
#define App_EnergyMonitor_GetHours(time)                 (time / (APP_ENERGY_MONITOR_MIN_IN_H * APP_ENERGY_MONITOR_S_IN_MIN * \
                                                            APP_ENERGY_MONITOR_TICKS_IN_S))

/*!	
 * \brief Macro converts global time into minutes
 *
 * \param[in] time Global time in miliseconds
 * \param[in] hours Number of hours elapsed since system start-up
 * 
 * \retval Number of minutes elapsed since system start-up (0-60)
 */
#define App_EnergyMonitor_GetMinutes(time, hours)        ((time / (APP_ENERGY_MONITOR_S_IN_MIN * APP_ENERGY_MONITOR_TICKS_IN_S)) - \
                                                            (hours * APP_ENERGY_MONITOR_MIN_IN_H))

/*!	
 * \brief Macro converts global time into seconds
 *
 * \param[in] time Global time in miliseconds
 * \param[in] minutes Number of minutes elapsed since system start-up
 * 
 * \retval Number of seconds elapsed since system start-up (0-60)
 */
#define App_EnergyMonitor_GetSeconds(time, minutes)      ((time / APP_ENERGY_MONITOR_TICKS_IN_S) - \
                                                            (minutes * APP_ENERGY_MONITOR_S_IN_MIN))

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

typedef struct
{
    float bus_voltage;          /* V */
    float current;              /* mA */
    float power;                /* mW */
    float power_consumption;    /* mWh: milliwatt-hour */
    bool alert_status;
}App_EnergyMonitor_Data_t;


/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

static void App_EnergyMonitor_Task(void const * argument);
static void App_EnergyMonitor_UpdateData(void);
static void App_EnergyMonitor_ReadAlertStatus(void);
static void App_EnergyMonitor_UpdateLeds(void);
static void App_EnergyMonitor_TransmitLog(void);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static osThreadId App_EnergyMonitor_TaskHandle;
static App_EnergyMonitor_Data_t App_EnergyMonitor_Data;
static char App_EnergyMonitor_Log[APP_ENERGY_MONITOR_LOG_LEN];

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

/*!	
 * \brief APP energy monitor initialization function
 *
 * \param[in] None
 * 
 * \retval None
 */
void App_EnergyMonitor_Init(void)
{
    /* Create thread */
    osThreadDef(App_EnergyMonitor, App_EnergyMonitor_Task, osPriorityAboveNormal, 0, 512);
    App_EnergyMonitor_TaskHandle = osThreadCreate(osThread(App_EnergyMonitor), NULL);
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/

/*!	
 * \brief APP energy monitor task
 *
 * \param[in] argument OS required parameter
 * 
 * \retval None
 */
static void App_EnergyMonitor_Task(void const * argument)
{
    uint32_t time = osKernelSysTick();

    while(1)
    {
        App_EnergyMonitor_UpdateData();
        App_EnergyMonitor_ReadAlertStatus();
        App_EnergyMonitor_UpdateLeds();
        App_EnergyMonitor_TransmitLog();

        osDelayUntil(&time, APP_ENERGY_MONITOR_THREAD_PERIOD);
    }
}

/*!	
 * \brief The function updates the required variables
 *
 * \param[in] None
 * 
 * \retval None
 */
static void App_EnergyMonitor_UpdateData(void)
{
    Hal_EnergyMonitor_Data_t data;

    Hal_EnergyMonitor_GetResults(&data);
    App_EnergyMonitor_Data.bus_voltage = data.bus_voltage;
    App_EnergyMonitor_Data.power = data.power;
    App_EnergyMonitor_Data.current = data.current;

    /* Conversion to milliwatt-hour */
    App_EnergyMonitor_Data.power_consumption += App_EnergyMonitor_Data.power * \
                                                ((float)(APP_ENERGY_MONITOR_THREAD_PERIOD / APP_ENERGY_MONITOR_TICKS_IN_S)) / \
                                                (float)APP_ENERGY_MONITOR_S_IN_MIN / (float)APP_ENERGY_MONITOR_MIN_IN_H;
}

/*!	
 * \brief The function updates alert status
 *
 * \param[in] None
 * 
 * \retval None
 */
static void App_EnergyMonitor_ReadAlertStatus(void)
{
    App_EnergyMonitor_Data.alert_status = Hal_Gpio_GetAlertStatus();
}

/*!	
 * \brief The function updates the status of the LEDs. When an alarm occurs, one LED 
 *        is turned on. When another alarm occurs, another LED is turned on.
 *
 * \param[in] None
 * 
 * \retval None
 */
static void App_EnergyMonitor_UpdateLeds(void)
{
    static Hal_Gpio_Led_t led = Hal_Gpio_LedGreen;

    if(App_EnergyMonitor_Data.alert_status)
    {
        Hal_Gpio_LedOn(led);
    }
    else
    {
        Hal_Gpio_LedOff(led);

        led++;

        if(led >= Hal_Gpio_LedMax)
        {
            led = Hal_Gpio_LedGreen;
        }
    }
}

/*!	
 * \brief The function tranmit log via serial port. Data to be transmitted:
 *        - time since system start-up
 *        - bus voltage
 *        - current
 *        - power
 *        - power consumption
 *        - alert status
 *
 * \param[in] None
 * 
 * \retval None
 */
static void App_EnergyMonitor_TransmitLog(void)
{
    uint32_t time_global = osKernelSysTick();
    uint8_t time_h = App_EnergyMonitor_GetHours(time_global);
    uint8_t time_min = App_EnergyMonitor_GetMinutes(time_global, time_h);
    uint8_t time_s = App_EnergyMonitor_GetSeconds(time_global, time_min);

    memset(App_EnergyMonitor_Log, '\0', sizeof(App_EnergyMonitor_Log));

    sprintf(App_EnergyMonitor_Log, "%.2d::%.2d::%.2d U= %.2f[V] I=%.2f [mA] P=%.2f [mW] Consumption=%.2f [mWh] Alert:%d\r\n", \
            time_h, time_min, time_s, App_EnergyMonitor_Data.bus_voltage, App_EnergyMonitor_Data.current, \
            App_EnergyMonitor_Data.power, App_EnergyMonitor_Data.power_consumption, App_EnergyMonitor_Data.alert_status);

    (void)Hal_Uart_Write((uint8_t*)App_EnergyMonitor_Log, sizeof(App_EnergyMonitor_Log));
}
