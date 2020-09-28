//#include "lpc177x_8x_libcfg_default.h"
#include "Protocol_CAN.h"
#include "lpc177x_8x_can.h"
#include "debug_frmwrk.h"
#include "delay.h"
#include "CarCommProtocol.h"
#include "Protocol_KWP.h"
#include "ECU_helper.h"
#include "Uart_InterruptManager.h"
#include "RelayController.h"
#include "log_Helper.h"

extern volatile uint8_t KWPRxCountFlag;
extern	volatile uint8_t m_iFlagRecieveFrame;
extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern Can_Device m_canDev; 
extern uint8_t m_SendStateForUart2;
extern ProtocolProfiler * m_CurrentProf;
extern uint8_t m_LogEnabled;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
extern char check_For_rec;
extern volatile uint8_t m_ConnectionStatus;
extern volatile uint8_t m_FirstPromptDC;
uint8_t m_SendFrameBuffer[384];
uint16_t m_NumberOfBuffer;
extern volatile uint8_t m_iFlagRecieveUARTFrame;
uint8_t KWPRxCount;
extern uint8_t ConnectionStatus;
extern uint16_t m_NumberOfSendBytes;
extern UART_RING_BUFFER_T rb2;
extern	ProtocolProfiler * m_CurrentProf;
extern	const taskDiag task[];
extern volatile uint8_t m_FirstPromptDC;
extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
extern volatile char reflect_condition;
unsigned char ecuStatusProgrammed[5] = {0x60,0x60,0x60,0x60,0x60};

PROTOCOL_SOLVER PROTOCOL_KWP_SOLVER = {
	ConnectToKWPDevice,
	SendAndRecieveKWPFrame,
	CheckTheResultsKWPAndPerformAction,
	isReadyFrameKWP
};


void ResetKWPCountRxTx(void)
{
	KWPRxCountFlag = 0;
	m_iFlagRecieveUARTFrame = 0;
	KWPRxCount = 0;
	check_For_rec = 0;
	//if(total_rec_reset == 1)
	//	CANRxCount = 0;
	//CANRxCount = 0;
	//CANTxCount = 0;
	//CANRxCountFlag = 0;
	//CANErrCount = 0;
	//IsCANCompletedOrNot = 0;
	//m_iFlagRecieveFrame = 0;
	
}


uint8_t ConnectToKWPDevice(DEVICE_TYPES * Spec)
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

	Activate_OBDPin(kwp_spec->pin_config->rec_pin, OFF_MODE);

	
	Activate_OBDPin(kwp_spec->pin_config->rec_pin, ON_RX_MODE);
	Activate_OBDPin(kwp_spec->pin_config->txd_pin, ON_TX_MODE);
	

	PINSEL_ConfigPin(U2_UART_PORT, U2_UART_TXD, 0);
	GPIO_SetDir(U2_UART_PORT,(1 << U2_UART_TXD),1);
	GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
	
	
	//short section:
	if(kwp_spec->init_type == INIT_SHORT_TYPE)
	{
		if(kwp_spec->short_init->l_line_executor == L_LINE_ON)
		{
			Activate_OBDPin(kwp_spec->short_init->l_line_pin, ON_TX_MODE);
			delay_ms(50);
		}
		delay_ms(50);
		
		GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
		GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(22);
		
	//	Activate_OBDPin(kwp_spec->short_init->l_line_pin, OFF_MODE);
		
		Server_Port_Config2(kwp_spec->serial_port_cofig->baud_rate);
	  UART_TxCmd(UART_2, ENABLE);
		
	}

	
	if(kwp_spec->init_type == INIT_LONG_TYPE)
	{
		int i = 0;

		if(kwp_spec->long_init->l_line_executor == L_LINE_ON)
		{
			Activate_OBDPin(kwp_spec->short_init->l_line_pin, ON_TX_MODE);
					delay_ms(50);

		}

		delay_ms(100);
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
		
		
		UART_SendByte(UART_2, 0x70);
		UART_Receive(UART_2, temp, 2, BLOCKING, 2000);
		
		//Activate_OBDPin(kwp_spec->long_init->l_line_pin, OFF_MODE);
		delay_ms(150);
		UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);		
	}

	return result;
}

uint16_t ConstructTheKWPFrmShort(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//81 11 F1 81 04
	int i =0;
	uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = 0x80 + sizeBuf;
		m_SendFrameBuffer[1] = kwpDevice->ID_part[0];
		m_SendFrameBuffer[2] = kwpDevice->ID_part[1];
		
		array_copy(m_SendFrameBuffer,3,2+sizeBuf,buf,0,sizeBuf-1);
		
		
		check_sum = 0;
		
		for (i = 0 ; i < 3+sizeBuf ; i++)
		{
			check_sum += m_SendFrameBuffer[i];
		}
		
		m_SendFrameBuffer[3+sizeBuf] = check_sum;
		
		return (4+sizeBuf);
		
}

uint16_t ConstructTheKWPFrmIMMO(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//81 11 F1 81 04
	int i =0;
	uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = sizeBuf;

		
		//array_copy(m_SendFrameBuffer,1,2+sizeBuf,buf,0,sizeBuf-1);
	memcpy(m_SendFrameBuffer+1, buf, sizeBuf);
		
		
		check_sum = 0;
		
		for (i = 0 ; i < 1+sizeBuf ; i++)
		{
			check_sum += m_SendFrameBuffer[i];
		}
		
		m_SendFrameBuffer[1+sizeBuf] = check_sum;
		
		return (2+sizeBuf);
		
}



uint16_t ConstructTheKWPFrmLong(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//80 58 F1 01 3E 08
	int i =0;
	uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = 0x80;
		m_SendFrameBuffer[1] = kwpDevice->ID_part[0];
		m_SendFrameBuffer[2] = kwpDevice->ID_part[1];
		
		m_SendFrameBuffer[3] = sizeBuf;
	
		array_copy(m_SendFrameBuffer,4,3+sizeBuf,buf,0,sizeBuf-1);
		
		
		check_sum = 0;
		
		for (i = 0 ; i < 4+sizeBuf ; i++)
		{
			check_sum += m_SendFrameBuffer[i];
		}
		
		m_SendFrameBuffer[4+sizeBuf] = check_sum;
		
		return (5+sizeBuf);
		
}



uint16_t ConstructTheKWPFrm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
	switch (kwpDevice->protocol_type)
	{
	case KWP_TYPE_SHORT:
		return ConstructTheKWPFrmShort(kwpDevice, buf, sizeBuf);
		break;
		
	case KWP_TYPE_LONG:
	return ConstructTheKWPFrmLong(kwpDevice, buf, sizeBuf);
		break;
		
	case KWP_TYPE_IMMO:
		return ConstructTheKWPFrmIMMO(kwpDevice, buf, sizeBuf);
		break;
	}
}

  //switch (m_Scheduler_Buffer.CMD)
  //{
  //case CMD2_TASK_DATA_CONNECT:

uint8_t SendAndRecieveKWPFrame(DEVICE_TYPES * Dev,	uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
	int i=0;
	int j=0;
	char wait_condition = 0;
	char continue_waiting =0;
	Kwp_Device * kwp_spec = Dev->Kwp_spect;
	
	ResetKWPCountRxTx();
	UART_TxCmd(UART_2, ENABLE);
	
	if(m_Scheduler_Buffer.CMD == CMD2_TASK_DATA_CONNECT)
	{
		//check that the initialize is long or not :
		if(kwp_spec->short_init->type_of_initialize_connect_frame == TYPE_CONNECTION_SHORT)
		{
			m_NumberOfBuffer = ConstructTheKWPFrmShort(kwp_spec,send_buffer, size_of_buffer);
		}
		else if(kwp_spec->short_init->type_of_initialize_connect_frame == TYPE_CONNECTION_LONG)
		{
			m_NumberOfBuffer = ConstructTheKWPFrm(kwp_spec,send_buffer, size_of_buffer);
		}
		else if(kwp_spec->short_init->type_of_initialize_connect_frame == TYPE_CONNECTION_IMMO)
		{
			m_NumberOfBuffer = ConstructTheKWPFrmIMMO(kwp_spec,send_buffer, size_of_buffer);
		}
	}
	else
	{
			m_NumberOfBuffer = ConstructTheKWPFrm(kwp_spec,send_buffer, size_of_buffer);
	}
	
		
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
	
	ResetKWPCountRxTx();
	reflect_condition = 1;
	m_NumberOfSendBytes = m_NumberOfBuffer; 
	memset(m_frmBank[bank_frm_num].frm, 0, 100);
	
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
	
	/*
	if(m_frmBank[bank_frm_num].frm[0] == 0x80)
	{
			for (i = 0,j=0 ; i < rb2.rx_head ; i++,j++)
			{
				if(i != 3)
				{
					m_frmBank[bank_frm_num].frm[j] = rb2.rx[i];
				}
				else
				{
					j--;
				}
			}
			m_frmBank[bank_frm_num].rec_byte_number = rb2.rx_head;
	}
	else
	{
		for (i = 0 ; i < rb2.rx_head ; i++)
		{
			m_frmBank[bank_frm_num].frm[i] = rb2.rx[i];
		}
		m_frmBank[bank_frm_num].rec_byte_number = rb2.rx_head;
		
	}
	
	*/
	
	
	
	
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

uint8_t CheckTheResultsKWPAndPerformAction(uint8_t status)
{
	int res= 0;
	taskDiagRAM tskDg;
	taskDiagRAM * tskPointer = &tskDg;
	
	//if(status == STATUS_ECU_CONNECTION_7F || status == STATUS_ECU_CONNECTION_LOST)
	if(status == STATUS_ECU_CONNECTION_LOST)
	{		
		int temp_cmd = m_Scheduler_Buffer.CMD;
		m_Scheduler_Buffer.CMD = CMD2_TASK_DATA_CONNECT;
		
		
		m_ConnectionStatus = CONNECTION_TO_CAR_NO;
		
		delay_ms(1500);
		delay_ms(1500);
		delay_ms(1500);
		delay_ms(1500);
		res = GeneralInitConnectProcedure(1);
		
		if(m_Scheduler_Buffer.CMD != CMD2_TASK_DATA_DISCONNECT)
			m_Scheduler_Buffer.CMD = temp_cmd;
		//now if the connection is not valid send the result to the device!!!!
		
		
		//check now the connection is okay or not:
		if(res == STATUS_ECU_CONNECTION_LOST)
		{
			char buffer_out[10];
			char j = 0;
			
			
			Activate_LEDPin(LED_CONNECT,OFF_MODE);
			
			
			if(m_FirstPromptDC == 0)
			{
				j = 0;
				buffer_out[j++] = m_ConnectionStatus;
	
				spi1_send_frame(XVBAT,m_Scheduler_Buffer.input_buf[1],0);
				spi1_recieve_frame();
          
				buffer_out[j++] =  xmegarx_frame[3];
				buffer_out[j++] =  xmegarx_frame[4];
	      
				/*
				if(ecuStatusProgrammed[0] == 1 && ecuStatusProgrammed[1] == 1)
					buffer_out[j++] = 0x00;
				else
					buffer_out[j++] = 0x60;
			   */
				
				buffer_out[j++] =  0x60;
				
				WriteTheFrame(UART_1, CMD1_TASK_DATA, temp_cmd , buffer_out , 4);
				
				m_FirstPromptDC = 1;
			}
			

			
			
			//SendStatusToDevice();
			//delay_ms(1000);
			//now should reset:
			
			//#ifdef __RAM_MODE__
			//Reset_Arm(10);
			//#endif
		}
		
		if(res == STATUS_ECU_CONNECTION_OKAY)
		{
			m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
			Activate_LEDPin(LED_CONNECT,ON_MODE);
		}
	}
	
	
	
	return 0;
}

uint8_t isReadyFrameKWPShort(uint8_t * tx, uint32_t tx_head)
{
	if(tx[0] != 0x80)
	{
		if(tx_head == tx[0] - 0x7c)
			return 1;
		else
			return 0;		
	}
	else
	{
		if(tx[0] == 0x80)
		{
			if(tx_head == tx[3] + 5)
				return 1;
			else
				return 0;
		}
	}
}

uint8_t isReadyFrameKWPIMMO(uint8_t * tx, uint32_t tx_head)
{
	if(isReadyFrameKWPShort(tx, tx_head) == 1)
		return 1;
	
	if(tx[0]+2 == tx_head)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}



uint8_t isReadyFrameKWP(uint8_t * tx, uint32_t tx_head)
{
	switch (m_CurrentProf->spect->Kwp_spect->protocol_type)
	{
	case KWP_TYPE_SHORT:
	case KWP_TYPE_LONG:
		return isReadyFrameKWPShort(tx, tx_head);
		break;
		
		
	case KWP_TYPE_IMMO:
		return isReadyFrameKWPIMMO(tx, tx_head);
		//return isReadyFrameKWPImmo(tx, tx_head);
		break;
	}
}

	/*
	if(m_iFlagRecieveUARTFrame == 0)
	{
	wait_condition = 0;
	continue_waiting =0;
	
	while(continue_waiting == 0)
	{
		
	if(kwp_spec->protocol_type == KWP_TYPE_IMMO)
	{
		if(rb2.rx[1] == 0x7f && rb2.rx[3] == 0x78)
				wait_condition = 1;		
	}
	else
	{
		if(rb2.rx[0] > 0x80)
		{
			if(rb2.rx[3] == 0x7f && rb2.rx[5] == 0x78)
				wait_condition = 1;
		} else if (rb2.rx[0] == 0x80)
		{
			if(rb2.rx[4] == 0x7f && rb2.rx[6] == 0x78)
				wait_condition = 1;
		}
	}
	
	if(wait_condition == 1)
	{
		  reflect_condition = 0;
			memset(m_frmBank[bank_frm_num].frm, 0, 100);
			Reset_UartRingBuffer2();
			KWPRxCountFlag = 0;
			check_For_rec = 1;
			m_iFlagRecieveUARTFrame = 0;
			delay_ms_interrupt(kwp_spec->serial_port_cofig->TimeOut);	
			while((!m_iFlagRecieveUARTFrame) & (!KWPRxCountFlag));
				Disable_Timer0();	
	if(m_iFlagRecieveUARTFrame == 1)
	{
		break;
	}
	
	}
	else
	{
		continue_waiting = 1;
	}

	}		
	}
	*/


