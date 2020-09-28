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

extern	uint8_t m_iFlagRecieveFrame;
extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern Can_Device m_canDev; 
extern uint8_t m_SendStateForUart2;
extern ProtocolProfiler * m_CurrentProf;
extern uint8_t m_LogEnabled;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

uint8_t m_SendFrameBuffer[384];
uint16_t m_NumberOfBuffer;
extern volatile uint8_t m_iFlagRecieveUARTFrame;
volatile uint8_t KWPRxCountFlag;
uint8_t KWPRxCount;
uint8_t ConnectionStatus=0;
extern uint16_t m_NumberOfSendBytes;
extern UART_RING_BUFFER_T rb2;
extern	ProtocolProfiler * m_CurrentProf;
extern	const taskDiag task[];


PROTOCOL_SOLVER PROTOCOL_KWP_SOLVER = {
	ConnectToKWPDevice,
	SendAndRecieveKWPFrame,
	CheckTheResultsKWPAndPerformAction
};


void ResetKWPCountRxTx(void)
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


uint8_t ConnectToKWPDevice(DEVICE_TYPES * Spec)
{
	uint32_t i;
	volatile uint32_t cnt;
	
	Kwp_Device * kwp_spec = Spec->Kwp_spect;
	
	Activate_ProtocolPin(PROTOCOL_KWP_RELAY, ON_MODE);
	
	
	if(kwp_spec->XOR_TXD == 0)
	{
		Activate_XORPin(XOR_TX, OFF_MODE);
	}
	else
	{
		Activate_XORPin(XOR_TX, ON_MODE);
	}
	
	if(kwp_spec->XOR_RXD == 0)
	{
		Activate_XORPin(XOR_RX, OFF_MODE);
	}
	else
	{
		Activate_XORPin(XOR_RX, ON_MODE);
	}

	
	Activate_OBDPin(kwp_spec->rec_pin, ON_MODE);
	Activate_OBDPin(kwp_spec->txd_pin, ON_MODE);
	

	PINSEL_ConfigPin(U2_UART_PORT, U2_UART_TXD, 0);
	GPIO_SetDir(U2_UART_PORT,(1 << U2_UART_TXD),1);
	GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
	
	if(kwp_spec->type_of_init == INIT_SHORT_TYPE)
	{
		GPIO_ClearValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
		GPIO_SetValue(U2_UART_PORT, (1 << U2_UART_TXD));
		delay_ms(25);
	}
	
	Server_Port_Config2(kwp_spec->baud_rate);
	UART_TxCmd(UART_2, ENABLE);
	
	return 0;
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
	switch (kwpDevice->type_of_protocol)
	{
	case KWP_TYPE_SHORT:
		return ConstructTheKWPFrmShort(kwpDevice, buf, sizeBuf);
		break;
		
	case KWP_TYPE_LONG:
	return ConstructTheKWPFrmLong(kwpDevice, buf, sizeBuf);
		break;
		
	case KWP_TYPE_IMMO:
		break;
	}
}

  //switch (m_Scheduler_Buffer.CMD)
  //{
  //case CMD2_TASK_DATA_CONNECT:

uint8_t SendAndRecieveKWPFrame(DEVICE_TYPES * Dev,	uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
	int i=0;
	Kwp_Device * kwp_spec = Dev->Kwp_spect;
	
	ResetKWPCountRxTx();
	UART_TxCmd(UART_2, ENABLE);
	
	if(m_Scheduler_Buffer.CMD == CMD2_TASK_DATA_CONNECT)
	{
		if(kwp_spec->type_of_connect_type == TYPE_CONNECTION_SHORT)
		{
			m_NumberOfBuffer = ConstructTheKWPFrmShort(kwp_spec,send_buffer, size_of_buffer);
		}
		else if(kwp_spec->type_of_connect_type == TYPE_CONNECTION_LONG)
		{
			m_NumberOfBuffer = ConstructTheKWPFrm(kwp_spec,send_buffer, size_of_buffer);
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
	
	
	m_NumberOfSendBytes = m_NumberOfBuffer; 
	Write_ModeBusFrame(UART_2, m_SendFrameBuffer, m_NumberOfBuffer);
	
	delay_ms_interrupt(kwp_spec->TimeOut);	
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

uint8_t CheckTheResultsKWPAndPerformAction(uint8_t status)
{
	int res= 0;
	taskDiagRAM tskDg;
	taskDiagRAM * tskPointer = &tskDg;
	
	//if(status == STATUS_ECU_CONNECTION_7F || status == STATUS_ECU_CONNECTION_LOST)
	if(status == STATUS_ECU_CONNECTION_LOST)
	{
		m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect); 
		
		ConvertTask(task[CONNECT], tskPointer);
		
		res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																	tskPointer->frmFormat[0].frm, 
																																	tskPointer->frmFormat[0].frm_size,0);
		
		
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

uint8_t isReadyFrameKWP(uint8_t * tx, uint32_t tx_head)
{
	switch (m_CurrentProf->spect->Kwp_spect->type_of_protocol)
	{
	case KWP_TYPE_SHORT:
		return isReadyFrameKWPShort(tx, tx_head);
		break;
		
	case KWP_TYPE_LONG:
		return isReadyFrameKWPShort(tx, tx_head);
		break;
		
	case KWP_TYPE_IMMO:
		//return isReadyFrameKWPImmo(tx, tx_head);
		break;
	}
}


