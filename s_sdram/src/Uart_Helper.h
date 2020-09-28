#ifndef __UART_HELPER_
#define __UART_HELPER_

#include "LPC177x_8x.h"
#include "stdint.h"
#include "lpc_types.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_pinsel.h"


int Initialize_UART_WithTXDMA(LPC_UART_TypeDef* UARTPort, uint8_t Buffer_tx[], uint16_t size_buffer_tx);
int Initialize_UART_NoDMA(UART_ID_Type UartID);
void Server_Port_Config0(void);
void Server_Port_Config1(int baud_rate);
void Server_Port_Config2(int baud_rate);
void Write_ModeBusFrame(UART_ID_Type UartID, uint8_t* pyFrame, int nNumByte);
void Write_ModeBusFrame_WithReflect(UART_ID_Type UartID, uint8_t* pyFrame, int nNumByte);
#endif
