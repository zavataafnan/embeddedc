//#include "lpc177x_8x_libcfg_default.h"
#include "Protocol_CAN.h"
#include "lpc177x_8x_can.h"
#include "debug_frmwrk.h"
#include "delay.h"
#include "CarCommProtocol.h"
#include "Protocol_SL96.h"
#include "ECU_helper.h"
#include "Uart_InterruptManager.h"
#include "RelayController.h"
#include "log_Helper.h"

extern	volatile uint8_t m_iFlagRecieveFrame;
extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern Can_Device m_canDev; 
extern uint8_t m_SendStateForUart2;
extern ProtocolProfiler * m_CurrentProf;
extern uint8_t m_LogEnabled;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

extern uint8_t m_SendFrameBuffer[384];
extern uint16_t m_NumberOfBuffer;
extern volatile uint8_t m_iFlagRecieveUARTFrame;
extern volatile uint8_t KWPRxCountFlag;
extern uint8_t KWPRxCount;
extern uint8_t ConnectionStatus;
extern uint16_t m_NumberOfSendBytes;
extern UART_RING_BUFFER_T rb2;
extern	ProtocolProfiler * m_CurrentProf;
extern	const taskDiag task[];


PROTOCOL_SOLVER PROTOCOL_SL96_SOLVER = {
	ConnectToSL96Device,
	SendAndRecieveSL96Frame,
	CheckTheResultsSL96AndPerformAction,
	isReadyFrameSL96
};


void ResetSL96CountRxTx(void)
{
	KWPRxCountFlag = 0;
	m_iFlagRecieveUARTFrame = 0;
	KWPRxCount = 0;
	//if(total_rec_reset == 1)
	//	CANRxCount = 0;
	//CANRxCount = 0;
	//CANTxCount = 0;
	//CANRxCountFlag = 0;
	//CANErrCount = 0;
	//IsCANCompletedOrNot = 0;
	//m_iFlagRecieveFrame = 0;
	
}


uint8_t ConnectToSL96Device(DEVICE_TYPES * Spec)
{
	uint32_t i;
	unsigned char temp[32];
	int rLen;
	
	volatile uint32_t cnt;
	
	Kwp_Device * kwp_spec = Spec->Kwp_spect;
	
	Activate_ProtocolPin(PROTOCOL_KWP_RELAY, ON_MODE);
	
	
	if(kwp_spec->pin_config->XOR_TXD == 0)
	{
		Activate_XORPin(XOR_TX, OFF_MODE);
	}
	else
	{
		Activate_XORPin(XOR_TX, ON_MODE);
	}
	
	if(kwp_spec->pin_config->XOR_RXD == 0)
	{
		Activate_XORPin(XOR_RX, OFF_MODE);
	}
	else
	{
		Activate_XORPin(XOR_RX, ON_MODE);
	}

	
	Activate_OBDPin(kwp_spec->pin_config->rec_pin, ON_MODE);
	Activate_OBDPin(kwp_spec->pin_config->txd_pin, ON_MODE);
	

	PINSEL_ConfigPin(U2_UART_PORT, U2_UART_TXD, 0);
	GPIO_SetDir(U2_UART_PORT,(1 << U2_UART_TXD),1);
	GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
	
	if(kwp_spec->init_type == INIT_SHORT_TYPE)
	{
		GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
		GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
	}
	
	if(kwp_spec->init_type == INIT_LONG_TYPE)
	{
		int i = 0;
		
		//construct the signal 
		GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(200);
		
		for (i = 7 ; i >= 0 ; i--)
		{
				if(checkbiter(kwp_spec->long_init->initialize_byte,i))
				{
					GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
					delay_ms(200);
				}
				else
				{
					GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
					delay_ms(200);
				}
		}
		
		//GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
		//delay_ms(200);
		
	}
	
	GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
	
	Server_Port_Config2(kwp_spec->serial_port_cofig->baud_rate);
	UART_TxCmd(UART_2, ENABLE);
	
				/* Enable UART Rx interrupt */
			UART_IntConfig(UART_2, UART_INTCFG_RBR, DISABLE);
			rLen = UART_Receive(UART_2, temp, 3, BLOCKING, 2000);
	
	

			
			
	
	///////////////////////////////////////////////////////////////////////
	
	if(kwp_spec->long_init->init_ecu_type == ECU_SL96)
	{

	
			if(rLen == 3)
			{
				rLen = UART_Receive(UART_2, temp, 28, BLOCKING, 2000);
		
			}
			else
			{
			//return 1;
			}
	
		delay_ms(150);
		UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
	//
	//
		if(rLen == 28)
	{
		return STATUS_ECU_CONNECTION_OKAY;
	}
	}
	
	
	if(kwp_spec->long_init->init_ecu_type == ECU_ABS)
	{
	  if(rLen == 3)
		{
				UART_SendByte(UART_2, 0x70);
				UART_Receive(UART_2, temp, 1, BLOCKING, 2000);
				delay_ms(150);
				UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
			
			return STATUS_ECU_CONNECTION_OKAY;
		
	  }
	else
	{
			//return 1;
	}
	}

	
	return STATUS_ECU_CONNECTION_LOST;
}



uint16_t ConstructTheSL96Frm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//80 58 F1 01 3E 08
		int i =0;
		uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = 0x80 + sizeBuf + 1; //
		
		memcpy(m_SendFrameBuffer+1, buf, sizeBuf);	
		
		check_sum = 0;
		
		for (i = 0 ; i <= sizeBuf ; i++)
		{
			check_sum += m_SendFrameBuffer[i];
		}
		
		m_SendFrameBuffer[sizeBuf+1] = ~check_sum;
		
		return (sizeBuf+2);
}

  //switch (m_Scheduler_Buffer.CMD)
  //{
  //case CMD2_TASK_DATA_CONNECT:

uint8_t SendAndRecieveSL96Frame(DEVICE_TYPES * Dev,	uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
	int i=0;
	Kwp_Device * kwp_spec = Dev->Kwp_spect;
	
	ResetKWPCountRxTx();
	UART_TxCmd(UART_2, ENABLE);
	
	m_NumberOfBuffer = ConstructTheSL96Frm(kwp_spec,send_buffer, size_of_buffer);
	
		
	#ifdef LOGGING_SECTION
	if(m_LogEnabled && (m_Scheduler_Buffer.CMD != CMD2_TASK_DATA_STANDBY))
	{
		SaveTheFrameToLog(m_SendFrameBuffer, m_NumberOfBuffer);
	}
	
	if(m_Scheduler_Buffer.send_to_debug)
	{
			Write_ModeBusFrame(UART_0, m_SendFrameBuffer, m_NumberOfBuffer);
	}
						
	#endif
	
	
	m_NumberOfSendBytes = m_NumberOfBuffer; 
	Write_ModeBusFrame(UART_2, m_SendFrameBuffer, m_NumberOfBuffer);
	
	delay_ms_interrupt(kwp_spec->serial_port_cofig->TimeOut);	
	while((!m_iFlagRecieveUARTFrame) & (!KWPRxCountFlag));
		Disable_Timer0();

	if(m_iFlagRecieveUARTFrame == 1)
	{
#ifndef RELEASE_CODE
		//_DBG_("CAN TIMEOUT ...");
#endif
		return STATUS_ECU_CONNECTION_LOST;
	}
	
	for (i = 0 ; i < rb2.rx_head ; i++)
	{
		m_frmBank[bank_frm_num].frm[i] = rb2.rx[i];
	}
	m_frmBank[bank_frm_num].rec_byte_number = rb2.rx_head;
	
	
						#ifdef LOGGING_SECTION
						if(m_LogEnabled && (m_Scheduler_Buffer.CMD != CMD2_TASK_DATA_STANDBY))
						{
								SaveTheFrameToLog(rb2.rx,rb2.rx_head);
						}
						
						if(m_Scheduler_Buffer.send_to_debug)
						{
							Write_ModeBusFrame(UART_0, rb2.rx,rb2.rx_head);
						}
						#endif
						
						
	
	Reset_UartRingBuffer2();
	
	return STATUS_ECU_CONNECTION_OKAY;
 
}

uint8_t CheckTheResultsSL96AndPerformAction(uint8_t status)
{
	int res= 0;
	taskDiagRAM tskDg;
	taskDiagRAM * tskPointer = &tskDg;
	
	//if(status == STATUS_ECU_CONNECTION_7F || status == STATUS_ECU_CONNECTION_LOST)
	if(status == STATUS_ECU_CONNECTION_LOST)
	{
		m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect); 
		//now if the connection is not valid send the result to the device!!!!
		
		if(res == STATUS_ECU_CONNECTION_LOST)
		{
			//now alert the device that discounnect!!
			//char ret_frame[] = {0x1};
			//WriteTheFrame(UART_1, CMD1_TASK_STATUS, CMD2_TASK_DATA_STATUS , ret_frame , sizeof(ret_frame));
		}
		
		return 1;
	}
	
	
	
	return 0;
}


uint8_t isReadyFrameSL96(uint8_t * tx, uint32_t tx_head)
{
		if(tx_head == (tx[0] - 0xc0 + 1))
			return 1;
		
		return 0;
}


