
#ifndef _UART_INTERRUPT_MANAGER_2_H_
#define _UART_INTERRUPT_MANAGER_2_H_


#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "Uart_Helper.h"
 /************************** PRIVATE DEFINITIONS *************************/


extern UART_RING_BUFFER_T rb;

/************************** PRIVATE FUNCTIONS *************************/
void Reset_UartRingBuffer2(void);
void UART2_IRQHandler(void);                
void UART_IntReceive2(UART_ID_Type UartID);
void UART_IntErr2(uint8_t bLSErrType);
void UART2_IRQHandler2(void); 
uint32_t UARTReceive2(UART_ID_Type UartID, uint8_t *rxbuf, uint8_t buflen);
void UART_IntReceive2(UART_ID_Type UartID);

#endif
