#pragma once
#include "delay.h"
#include "lpc177x_8x_timer.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_can.h"
#include "ECU_definition.h"
/************************** PRIVATE FUNCTIONS *************************/
/* CAN interrupt service routine */


#define XOR_RXD_PORT 0
#define XOR_TXD_PORT 0
#define XOR_RXD_PIN  5
#define XOR_TXD_PIN  7

#define U2_UART_PORT 0

#define U2_UART_TXD 10
#define U2_UART_RXD 11

#define KWP_TYPE_SHORT 0
#define KWP_TYPE_LONG  1
#define KWP_TYPE_IMMO  2

#define INIT_LONG_TYPE 1
#define INIT_SHORT_TYPE 0

#define TYPE_CONNECTION_LONG 0
#define TYPE_CONNECTION_SHORT 1





uint8_t SendAndRecieveKWPFrame(DEVICE_TYPES * canDev, uint8_t * buffer, uint16_t size_of_buffer, uint8_t bank_frm_num);
uint8_t ConnectToKWPDevice(DEVICE_TYPES * Spec);
uint8_t CheckTheResultsKWPAndPerformAction(uint8_t status);
uint16_t ConstructTheKWPFrm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf);
uint8_t isReadyFrame(uint8_t * tx, uint32_t tx_head);
void ResetKWPCountRxTx(void);
