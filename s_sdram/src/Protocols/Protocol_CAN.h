#pragma once
#include "delay.h"
#include "lpc177x_8x_timer.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_can.h"
#include "ECU_definition.h"
/************************** PRIVATE FUNCTIONS *************************/
/* CAN interrupt service routine */





void ResetCANCountRxTx(uint8_t total_rec_reset);
void CAN_IRQHandler(void);
void CAN_SetupAFTable(void);
void TestCANWithFrame(void);
void ConstructTheCANFrm(Can_Device * canDevice, uint8_t * buf, uint16_t sizeBuf, uint8_t containing_size);
uint8_t SendAndRecieveCANFrame(DEVICE_TYPES * canDev, uint8_t * buffer, uint16_t size_of_buffer, uint8_t bank_frm_num);
uint8_t ConnectToCANDevice(DEVICE_TYPES * Spec);
void CAN_SetupAFTableArray(CAN_FILTERING_ARRAY * can_filter);
uint8_t CheckTheResultsCANAndPerformAction(uint8_t status);

