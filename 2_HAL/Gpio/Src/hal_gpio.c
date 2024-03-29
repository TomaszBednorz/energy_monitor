/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include "gpio.h"
#include "hal_gpio.h"
#include "hal_gpio_cfg.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/*
 * LED configuration
 */
typedef struct
{
    Hal_Gpio_Led_t led_name;
	uint16_t pin;
    GPIO_TypeDef* port;
}Hal_Gpio_Config_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static Hal_Gpio_Config_t Hal_Gpio_Config[Hal_Gpio_LedMax] =
{
    #define HAL_GPIO_CFG_LED(name, pin_val, port_val) { \
        .led_name=name, \
		.pin=pin_val, \
		.port=port_val},
		HAL_GPIO_CFG_TABLE
    #undef HAL_GPIO_CFG_LED
};

static bool Hal_Gpio_AlertStatus;

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

/*!	
 * \brief Turn on led
 *
 * \param[in] led Led
 * 
 * \retval None
 */
void Hal_Gpio_LedOn(Hal_Gpio_Led_t led)
{
    for(uint8_t i = 0; i < (uint8_t)Hal_Gpio_LedMax; i++)
    {
        if(led == i)
        {
            HAL_GPIO_SET_PIN(Hal_Gpio_Config[i].port, Hal_Gpio_Config[i].pin);
        }
    }
}

/*!	
 * \brief Turn off led
 *
 * \param[in] led Led
 * 
 * \retval None
 */
void Hal_Gpio_LedOff(Hal_Gpio_Led_t led)
{
    for(uint8_t i = 0; i < (uint8_t)Hal_Gpio_LedMax; i++)
    {
        if(led == i)
        {
            HAL_GPIO_RESET_PIN(Hal_Gpio_Config[i].port, Hal_Gpio_Config[i].pin);
        }
    }
}

/*!	
 * \brief Function return alert status
 *
 * \param[in] None
 * 
 * \retval true - alert occurred, otherwise false
 */
bool Hal_Gpio_GetAlertStatus(void)
{
    bool ret_val = Hal_Gpio_AlertStatus;
    Hal_Gpio_AlertStatus = false;

    return ret_val;
}

/*!	
 * \brief Alert callback - should be called from ISR
 *
 * \param[in] None
 * 
 * \retval None
 */
void Hal_Gpio_AlertCb(void)
{
    Hal_Gpio_AlertStatus = true;
    Hal_Gpio_LedOn(Hal_Gpio_LedBlue); 
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/
