//#include "lpc177x_8x_libcfg_default.h"
#include "Protocol_CAN.h"
#include "lpc177x_8x_can.h"
#include "debug_frmwrk.h"
#include "delay.h"
#include "CarCommProtocol.h"
#include "Protocol_Rio.h"
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


PROTOCOL_SOLVER PROTOCOL_RIO_SOLVER = {
	ConnectToRIODevice,
	SendAndRecieveRIOFrame,
	CheckTheResultsRIOAndPerformAction,
	isReadyFrameRIO
};


void ResetRIOCountRxTx(void)
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


uint8_t ConnectToRIODevice(DEVICE_TYPES * Spec)
{
		uint32_t i;
	volatile uint32_t cnt;
	char temp[10];
	int rLen = 0;
	char result =STATUS_ECU_CONNECTION_LOST;
	
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
	
	
	//short section:
	if(kwp_spec->init_type == INIT_SHORT_TYPE)
	{
		if(kwp_spec->short_init->l_line_executor == L_LINE_ON)
		{
			Activate_OBDPin(kwp_spec->short_init->l_line_pin, ON_TX_MODE);
		}
		delay_ms(300);
		
		GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
		GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
		
		//Activate_OBDPin(kwp_spec->short_init->l_line_pin, OFF_MODE);
		
		Server_Port_Config2(kwp_spec->serial_port_cofig->baud_rate);
	  UART_TxCmd(UART_2, ENABLE);
		
	}
	
	
	if(kwp_spec->init_type == INIT_LONG_TYPE)
	{
		int i = 0;

		if(kwp_spec->long_init->l_line_executor == L_LINE_ON)
		{
			Activate_OBDPin(kwp_spec->short_init->l_line_pin, ON_TX_MODE);
		}

		delay_ms(300);
		//construct the signal 
		GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
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
		
		Server_Port_Config2(kwp_spec->serial_port_cofig->baud_rate);
	
		UART_TxCmd(UART_2, ENABLE);
		
	  UART_IntConfig(UART_2, UART_INTCFG_RBR, DISABLE);
	  rLen = UART_Receive(UART_2, temp, 3, BLOCKING, 2000);

		if(rLen == 3)
			result = STATUS_ECU_CONNECTION_OKAY;
		
		
		UART_SendByte(UART_2, 0xf7);
		UART_Receive(UART_2, temp, 2, BLOCKING, 2000);
		
		Activate_OBDPin(kwp_spec->long_init->l_line_pin, OFF_MODE);
		delay_ms(150);
		UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);		
	}

	return result;

}



uint16_t ConstructTheRIOFrm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//68 6a f1 
		int i =0;
		uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = 0x68; //
	  m_SendFrameBuffer[1] = kwpDevice->ID_part[0];
	 m_SendFrameBuffer[2] = kwpDevice->ID_part[1];
		
		memcpy(m_SendFrameBuffer+3, buf, sizeBuf);	
		
    	
		
		for (i = 0 ; i < sizeBuf + 3 ; i++)
		{
			check_sum += m_SendFrameBuffer[i];
		}
		
		m_SendFrameBuffer[sizeBuf+3] = check_sum;
		
		return (sizeBuf+4);
}

  //switch (m_Scheduler_Buffer.CMD)
  //{
  //case CMD2_TASK_DATA_CONNECT:

uint8_t SendAndRecieveRIOFrame(DEVICE_TYPES * Dev,	uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
	int i=0;
	int rec_bytes = 0;
	int rec_bytes_num = 0;
	Kwp_Device * kwp_spec = Dev->Kwp_spect;
	
	ResetKWPCountRxTx();
	UART_TxCmd(UART_2, ENABLE);
	
	m_NumberOfBuffer = ConstructTheRIOFrm(kwp_spec,send_buffer, size_of_buffer);
	
		
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
	
	rec_bytes = rb2.rx_head;
	rec_bytes_num = rec_bytes;
	for (i = 0 ; i < rec_bytes ; i++)
	{
		m_frmBank[bank_frm_num].frm[i] = rb2.rx[i];
	}
	m_frmBank[bank_frm_num].rec_byte_number = rec_bytes;

	if(rb2.rx_head == 0x0b)
	{
		char temp[15];
		int rLen=0;
		UART_IntConfig(UART_2, UART_INTCFG_RBR, DISABLE);
		do 
		{
			rLen = UART_Receive(UART_2, temp, 11, BLOCKING, 150);
			
			for (i = 0 ; i < rLen ; i++)
			{
					m_frmBank[bank_frm_num].frm[i+m_frmBank[bank_frm_num].rec_byte_number] = temp[i];
			}
			m_frmBank[bank_frm_num].rec_byte_number += rLen;

		} while(rLen >= 11);
		
		rLen = 100;
		
		delay_ms(100);
		UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
	}
	
	
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

uint8_t CheckTheResultsRIOAndPerformAction(uint8_t status)
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
			return 1;
			//now alert the device that discounnect!!
			//char ret_frame[] = {0x1};
			//WriteTheFrame(UART_1, CMD1_TASK_STATUS, CMD2_TASK_DATA_STATUS , ret_frame , sizeof(ret_frame));
		}
		
		return 0;
	}
	
	
	
	return 0;
}


uint8_t isReadyFrameRIO(uint8_t * tx, uint32_t tx_head)
{
	int i = 0;
	Kwp_Device * kwp_spec;
	//if(tx_head == 12)
	//	return 1;
 if(kwp_spec->long_init->init_ecu_type == ECU_RIO)
 {
	if(tx_head > 5)
	{
		char ck=0;
		for (i = 0 ; i < tx_head-1 ; i++)
		{
			ck += tx[i];
		}
		
		if(tx[tx_head-1] == ck)
			return 1;
		else
			return 0;
	}
}
 else if (kwp_spec->long_init->init_ecu_type == ECU_RIO_ABS){
	 if(tx_head >= 5)
	{
		char ck=0;
		for (i = 0 ; i < tx_head-1 ; i++)
		{
			ck += tx[i];
		}
		
		if(tx[tx_head-1] == ck)
			return 1;
		else
			return 0;
	}
	
	 
	 
	 
 }
	
	return 0;
	/*
		if(tx_head == (tx[0] - 0xc0 + 1))
			return 1;
		
		return 0;
	*/
}


/*
uint8_t isReadyFrameRIO(uint8_t * tx, uint32_t tx_head)
{
	int i = 0;
	//if(tx_head == 12)
	//	return 1;

	if(tx_head >= 5)
	{
		char ck=0;
		for (i = 0 ; i < tx_head-1 ; i++)
		{
			ck += tx[i];
		}
		
		if(tx[tx_head-1] == ck)
			return 1;
		else
			return 0;
	}
	
	return 0;
	
		if(tx_head == (tx[0] - 0xc0 + 1))
			return 1;
		
		return 0;
	
}
*/


