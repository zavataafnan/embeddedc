/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/

#include <string.h>
#include "LPC177x_8x.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_adc.h"
#include "board.h"




/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define CONSOLE_DEV UART_0//((LPC_UART_TypeDef *)LPC_UART0)


/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *   Initialize the console (UART connected to USB-to-UART bridge)
 *
 *****************************************************************************/
void console_init(void)
{
  // UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;

  // pinsel for UART
	PINSEL_ConfigPin(0,2,1);
	PINSEL_ConfigPin(0,3,1);

	UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = 115200;

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init(CONSOLE_DEV, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd(CONSOLE_DEV, ENABLE);
}

/******************************************************************************
 *
 * Description:
 *   Send data to the console.
 *
 * Params:
 *   [in] txbuf - buffer containing data to send
 *   [in] buflen - number of bytes to send
 *   [in] flag - indicates blocking or non-blocking transfer
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_send(uint8_t *txbuf, uint32_t buflen,
		TRANSFER_BLOCK_Type flag)
{
	return UART_Send(CONSOLE_DEV, txbuf, buflen, flag);
}

/******************************************************************************
 *
 * Description:
 *   Send a null-terminates string to the console.
 *
 * Params:
 *   [in] str - the string to send
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_sendString(uint8_t *str)
{
	return UART_Send(CONSOLE_DEV, str, strlen((char*)str), BLOCKING);
}

/******************************************************************************
 *
 * Description:
 *   Receive a block of data from the console.
 *
 * Params:
 *   [in] rxbuf - pointer to receive buffer
 *   [in] buflen -
 *
 * Returns:
 *   Number of bytes sent.
 *
 *****************************************************************************/
uint32_t console_receive(uint8_t *rxbuf, uint32_t buflen,
		TRANSFER_BLOCK_Type flag)
{
	return UART_Receive(CONSOLE_DEV, rxbuf, buflen, flag, 500);
}

