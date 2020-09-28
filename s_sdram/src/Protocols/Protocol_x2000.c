//#include "lpc177x_8x_libcfg_default.h"
#include "Protocol_x2000.h"
#include "lpc177x_8x_can.h"
#include "debug_frmwrk.h"
#include "delay.h"
#include "CarCommProtocol.h"
#include "Protocol_X2000.h"
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
uint8_t counter_xantia =1 ;

PROTOCOL_SOLVER PROTOCOL_X2000_SOLVER = {
	ConnectToX2000Device,
	SendAndRecieveX2000Frame,
	CheckTheResultsX2000AndPerformAction,
	isReadyFrameX2000
};


void ResetX2000CountRxTx(void)
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

uint8_t rec_frame_iden[30];
uint8_t rec_frame_iden1[30];

uint8_t rec_xantia_2000_frame(DEVICE_TYPES * Spec, uint8_t * rec_frame)
{
	int rLen = 0;
	int i = 0;
	uint8_t temp = 0;
	Kwp_Device * kwp_spec = Spec->Kwp_spect;
	
	rLen = UART_Receive(UART_2, &rec_frame[0], 1, BLOCKING, kwp_spec->serial_port_cofig->TimeOut);
	
	if(rLen == 0)
		return STATUS_ECU_CONNECTION_LOST;
	
	delay_ms(4);
	UART_SendByte(UART_2, ~rec_frame[0]);
	UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
	
	for (i = 1; i < rec_frame[0] ; i++)
	{
		rLen = UART_Receive(UART_2, &rec_frame[i], 1, BLOCKING, 50);
		if(rLen == 0)
			return STATUS_ECU_CONNECTION_LOST;
		
		delay_ms(4);
		UART_SendByte(UART_2, ~rec_frame[i]);
		UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
	}
	
	//recieve 0x03
	UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
	rec_frame[rec_frame[0]] =  temp;
	
	counter_xantia++;
}

uint32_t UART_Send_x2000(UART_ID_Type UartID, uint8_t *txbuf,
                   uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
  uint32_t bToSend, bSent, timeOut, fifo_cnt;
  uint8_t *pChar = txbuf;
  __IO uint32_t *LSR = NULL;
	uint8_t temp=0;
	uint8_t rLen=0;
  
  switch (UartID)
  {
  case UART_0:
    LSR = (__IO uint32_t *)&LPC_UART0->LSR;
    break;
  case UART_1:
    LSR = (__IO uint32_t *)&LPC_UART1->LSR;
    break;
  case UART_2:
    LSR = (__IO uint32_t *)&LPC_UART2->LSR;
    break;
  case UART_3:
    LSR = (__IO uint32_t *)&LPC_UART3->LSR;
    break;
  case UART_4:
    LSR = (__IO uint32_t *)&LPC_UART4->LSR;
    break;
  }
  
  bToSend = buflen;
  
  // blocking mode
  if (flag == BLOCKING)
  {
    bSent = 0;
    while (bToSend)
    {
      timeOut = UART_BLOCKING_TIMEOUT;
      
      // Wait for THR empty with timeout
      while (!(*LSR & UART_LSR_THRE))
      {
        if (timeOut == 0)
          break;
        
        timeOut--;
      }
      
      // Time out!
      if(timeOut == 0)
        break;
      
      fifo_cnt = UART_TX_FIFO_SIZE;
      
      while (fifo_cnt && bToSend)
      {
        UART_SendByte(UartID, (*pChar++));
				UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
				UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
				
				delay_ms(6);

        fifo_cnt--;
        
        bToSend--;
        
        bSent++;
      }
    }
		
		UART_SendByte(UartID, 0x03);
		UART_Receive(UART_2, &temp, 1, BLOCKING, 50);
  }
  else
  {
    bSent = 0;
    while (bToSend)
    {
      if (bToSend == 0)
        break;
      
      if (!(*LSR & UART_LSR_THRE))
      {
        break;
      }
      
      fifo_cnt = UART_TX_FIFO_SIZE;
      
      while (fifo_cnt && bToSend)
      {
        UART_SendByte(UartID, (*pChar++));
        
        bToSend--;
        
        fifo_cnt--;
        
        bSent++;
      }
    }
  }
  
  return bSent;
}


void send_xantia_2000_frame(UART_ID_Type UartID, uint8_t* pyFrame, int nNumByte)
{
  LPC_UART_TypeDef * uart = LPC_UART0;
  UART_TxCmd(UartID, ENABLE);

	delay_ms(30);
  UART_Send_x2000(UartID, pyFrame, nNumByte, BLOCKING);
  
  
  switch (UartID)
  {
  case UART_0:
    uart = LPC_UART0;
    break;
  case UART_1:
    uart = (LPC_UART_TypeDef*)LPC_UART1;
    break;	
  case UART_2:
    uart = LPC_UART2;
    break;
  case UART_3:
    uart = LPC_UART3;
    break;
  case UART_4:
    uart = (LPC_UART_TypeDef*)LPC_UART4;
    break;
  }
  
  while (!(uart->LSR & UART_LSR_TEMT));
  
}


uint8_t ConnectToX2000Device(DEVICE_TYPES * Spec)
{
		uint32_t i;
	volatile uint32_t cnt;
	char temp[10];
	int rLen = 0;
	uint8_t frame_send[] = {0x9};
	
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
			Activate_OBDPin(kwp_spec->long_init->l_line_pin, ON_TX_MODE);
		}

		delay_ms(1000);
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
		
		delay_ms(6);
		
		UART_SendByte(UART_2, 0xfd);
		UART_Receive(UART_2, temp, 1, BLOCKING, 2000);
		
		rec_xantia_2000_frame(Spec, rec_frame_iden);
		
		delay_ms(250);
		send_xantia_2000_frame(UART_2, frame_send, 1);
		rec_xantia_2000_frame(Spec, rec_frame_iden1);
		//Activate_OBDPin(kwp_spec->long_init->l_line_pin, OFF_MODE);
		//delay_ms(150);
		//UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);		
	}

	return result;

}



uint16_t ConstructTheX2000Frm(Kwp_Device * kwpDevice, uint8_t * buf, uint16_t sizeBuf)
{
		//68 6a f1 
		int i =0;
		uint8_t check_sum = 0;
		m_SendFrameBuffer[0] = sizeBuf+2; //size of the frame
	  m_SendFrameBuffer[1] = counter_xantia++;
		
		memcpy(m_SendFrameBuffer+2, buf, sizeBuf);	
		//for (i = 0 ; i < sizeBuf + 3 ; i++)
		//{
		//	check_sum += m_SendFrameBuffer[i];
		//}
		
		//m_SendFrameBuffer[sizeBuf+2] = 0x03;
		
		return (sizeBuf+2);
}

  //switch (m_Scheduler_Buffer.CMD)
  //{
  //case CMD2_TASK_DATA_CONNECT:

uint8_t SendAndRecieveX2000Frame(DEVICE_TYPES * Dev,	uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
	int i=0;
	Kwp_Device * kwp_spec = Dev->Kwp_spect;
	
	ResetKWPCountRxTx();
	UART_TxCmd(UART_2, ENABLE);
	
	m_NumberOfBuffer = ConstructTheX2000Frm(kwp_spec,send_buffer, size_of_buffer);
	
		
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
	//Write_ModeBusFrame(UART_2, m_SendFrameBuffer, m_NumberOfBuffer);
	send_xantia_2000_frame(UART_2, m_SendFrameBuffer, m_NumberOfBuffer);
	
	
	if(rec_xantia_2000_frame(Dev, m_frmBank[bank_frm_num].frm) == 0)
	{
		return STATUS_ECU_CONNECTION_LOST;
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
						
						
	
	//Reset_UartRingBuffer2();
	
	return STATUS_ECU_CONNECTION_OKAY;
 
}

uint8_t CheckTheResultsX2000AndPerformAction(uint8_t status)
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


uint8_t isReadyFrameX2000(uint8_t * tx, uint32_t tx_head)
{
	return 0;
}


