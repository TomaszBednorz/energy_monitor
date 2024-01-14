#ifndef _HAL_UART_H_
#define _HAL_UART_H_

/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <stdint.h>

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Exported objects ********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************** Exported function prototypes ***************************************
 ***********************************************************************************************************/

/*
 * API
 */
void Hal_Uart_Init(void);
uint8_t Hal_Uart_Write(uint8_t* data, uint16_t size);

/*
 * Callbacks
 */
void Hal_Uart_WriteCb(void);

#endif  /* _HAL_UART_H_ */