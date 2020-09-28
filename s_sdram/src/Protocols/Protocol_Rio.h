#ifndef RIO_CODE_
#define RIO_CODE_

#include "delay.h"
#include "lpc177x_8x_timer.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_can.h"
#include "ECU_definition.h"
/************************** PRIVATE FUNCTIONS *************************/
/* CAN interrupt service routine */





uint8_t SendAndRecieveRIOFrame(DEVICE_TYPES * canDev, uint8_t * buffer, uint16_t size_of_buffer, uint8_t bank_frm_num);
uint8_t ConnectToRIODevice(DEVICE_TYPES * Spec);
uint8_t CheckTheResultsRIOAndPerformAction(uint8_t status);
uint16_t ConstructTheRIOFrm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf);
uint8_t isReadyFrameRIO(uint8_t * tx, uint32_t tx_head);
void ResetKWPCountRxTx(void);
#endif