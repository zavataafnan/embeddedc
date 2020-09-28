
#ifndef _UART_INTERRUPT_MANAGER_H_
#define _UART_INTERRUPT_MANAGER_H_


#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "Uart_Helper.h"
 /************************** PRIVATE DEFINITIONS *************************/

/* buffer size definition */
#define UART_RING_BUFSIZE 65536

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)  (bufidx=0)
#define __BUF_INCR(bufidx)  (bufidx=(bufidx+1)&__BUF_MASK)


/************************** PRIVATE TYPES *************************/
/** @brief UART Ring buffer structure */
typedef struct
{
    uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;

extern UART_RING_BUFFER_T rb;

/************************** PRIVATE FUNCTIONS *************************/
void Reset_UartRingBuffer(void);
void UART0_IRQHandler(void);                
void UART_IntReceive(UART_ID_Type UartID);
void UART_IntErr(uint8_t bLSErrType);
void UART2_IRQHandler(void); 
uint32_t UARTReceive(UART_ID_Type UartID, uint8_t *rxbuf, uint8_t buflen);


#endif
