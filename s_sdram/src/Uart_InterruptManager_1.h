
#ifndef _UART_INTERRUPT_MANAGER_1_H_
#define _UART_INTERRUPT_MANAGER_1_H_


#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "Uart_InterruptManager.h"
#include "Uart_Helper.h"
 /************************** PRIVATE DEFINITIONS *************************/



extern UART_RING_BUFFER_T rb1;

/************************** PRIVATE FUNCTIONS *************************/

void Reset_UartRingBuffer1(void);
void UART1_IRQHandler(void);                
void UART_IntReceive1(UART_ID_Type UartID);
void UART_IntErr1(uint8_t bLSErrType); 
uint32_t UARTReceive1(UART_ID_Type UartID, uint8_t *rxbuf, uint8_t buflen);

 
#endif
