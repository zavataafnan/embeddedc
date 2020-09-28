/**********************************************************************
* $Id: Mci_CidCard.c 28 2014-01-27 14:45:13Z danielru $		Mci_CidCard.c	2011-06-02
*//**
* @file		Mci_CidCard.c
* @brief	This example describes how to test MCI card
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "lpc_types.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "system_lpc177x_8x.h"
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "debug_frmwrk.h"
#include "diskio.h"
#include "ff.h"
#include "board.h"
#include "lpc177x_8x_eeprom.h"
#include "SecurityLevels.h"
#include "Protocol_CAN.h"
#include "CarCommProtocol.h"
#include "ECU_define.h"
#include "spi_helper.h"
#include "sdram.h"
#include "nand.h"
#include "RelayController.h"
#include "jsmn.h"
#include "Uart_Helper.h"
#include "ECU_helper.h"
#include "string.h"
#include "stdio.h"
#include "lpc177x_8x_nvic.h"
#include "stdlib.h" 
#include "EEPROMManage.h"
#include "analyzeDateTime.h"
#include "blowfish.h"
#include "GeneralFunction.h"
#include "version.h"
#include "BT_helper.h"
int version_ecu = ECU_NUMBER;
char firmware_Ver[10] = FIRMWARE_VER;

extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
extern int m_SecurityStatus;
extern SECURITY_ID m_SecurityID;
extern volatile uint8_t m_iFlagRecieveFrame;
extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
extern unsigned char m_State_Recieieved;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

typedef void (* func)(void);
#define USER_PROGRAM_START 0x10000000


extern int m_LevelSecurityStatus;
const ECU_LICENSES * ecu_license = (ECU_LICENSES *) 0x30000;
const uint8_t * security_flash_sector = (uint8_t *) 0x28000;


#define DMA_SIZE       (1000UL)
#define DMA_SRC			LPC_PERI_RAM_BASE		/* This is the area original data is stored
or data to be written to the SD/MMC card. */
#define DMA_DST			(DMA_SRC+DMA_SIZE)		/* This is the area, after writing to the SD/MMC,
data read from the SD/MMC card. */


#define PAGE_OFFSET			0x10
#define PAGE_ADDR			0x01


/* treat WriteBlock as a constant address */
volatile uint8_t *WriteBlock = (uint8_t *)(DMA_SRC);

/* treat ReadBlock as a constant address */
volatile uint8_t *ReadBlock  = (uint8_t *)(DMA_DST);
extern SCHEDULER * m_SchedulerPointer;
extern uint8_t m_jump_Condition;

#if MCI_DMA_ENABLED
/******************************************************************************
**  DMA Handler
******************************************************************************/
void DMA_IRQHandler (void)
{
  MCI_DMA_IRQHandler();
}
#endif


void HardFault_Handler(void)
{
	Buzzer_On(100);
	delay_ms(200);
	Buzzer_On(100);
	delay_ms(200);
	Buzzer_On(100);
	delay_ms(200);

	//while(1);
	Reset_Arm(10);
}


void SysTick_Handler(void)
{
  disk_timerproc();
  return;
}







void SystemConfiguration(void)
{
  //////////////////////////////////////////////////////////////////
#ifndef RELEASE_CODE  
  console_init();
  //console_sendString((uint8_t*)"Start the Serial\r\n");
  
  //for debug framework :::
  
  debug_frmwrk_init();
#endif
  
  
  Server_Port_Config0();
  //Server_Port_Config1(9600);
  
  UART_TxCmd(UART_1, ENABLE);

  
  
  nand_init();
  
  EEPROM_Init();
  
}



__asm void boot_jump( uint32_t address ){
   LDR SP, [R0]		;Load new stack pointer address
   LDR PC, [R0, #4]	;Load new program counter address
}

int main (void)
{
  
  char cc=0;
  int dc=0;
	int t;
  char tt;
  char rr;
	int i =0;
	int r_v;
	uint8_t * test_point;
	char t12=0;
	
	
	//#ifndef __NO_SDRAM__
	#ifndef __RAM_MODE__
  SystemConfiguration();
	#endif	
	
	ReadTheSecurityPatchToEEPROM(&m_SecurityID);
	Server_Port_Config1(m_SecurityID.bluetooth_baudrate);
	//#endif
	

  //Bluetooth_Set_Mode(BLUETOOTH_COMMAND_MODE);
	#ifndef __RAM_MODE__
	SetBluetooth_Command(921600);
	#endif
	
  //#ifdef TEST_JUMPING
		Buzzer_On(100);	
		Buzzer_On(100);

	//#endif 
  //
  //invert on/off mode of MCI power pin(we are using a P-mosfet)
  LPC_SC->SCS  = ((LPC_SC->SCS) & 0xFFFFFFF7);

  ResetTheBuffer();
  SPI_initialize_internal();
	
	__enable_irq();
  
  #ifndef __RAM_MODE__
  debug_frmwrk_init();
	#endif
	
	#ifdef __RAM_MODE__
	m_Scheduler_Buffer.CMD = CMD2_TASK_DATA_CONNECT;
	#endif
		

	while(1)
	{	
		Runner();
	}
	
	
	return 0;
}



/******************************************************************************
**                            End Of File
******************************************************************************/

