#ifndef X2000_CODE_
#define X2000_CODE_

#include "delay.h"
#include "lpc177x_8x_timer.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_can.h"
#include "ECU_definition.h"
/************************** PRIVATE FUNCTIONS *************************/
/* CAN interrupt service routine */





uint8_t SendAndRecieveX2000Frame(DEVICE_TYPES * canDev, uint8_t * buffer, uint16_t size_of_buffer, uint8_t bank_frm_num);
uint8_t ConnectToX2000Device(DEVICE_TYPES * Spec);
uint8_t CheckTheResultsX2000AndPerformAction(uint8_t status);
uint16_t ConstructTheX2000Frm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf);
uint8_t isReadyFrameX2000(uint8_t * tx, uint32_t tx_head);
void ResetKWPCountRxTx(void);
#endif