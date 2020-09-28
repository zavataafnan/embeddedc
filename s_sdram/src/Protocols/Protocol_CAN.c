
#include "Protocol_CAN.h"
#include "lpc177x_8x_can.h"
#include "debug_frmwrk.h"
#include "delay.h"
#include "CarCommProtocol.h"
#include "Protocol_CAN.h"
#include "ECU_helper.h"
#include "RelayController.h"
#include "lpc177x_8x_can.h"

extern  volatile uint8_t m_iFlagRecieveFrame;
extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern Can_Device m_canDev;
extern volatile uint8_t m_ConnectionStatus;
extern  ProtocolProfiler * m_CurrentProf;
extern  const taskDiag task[];
extern volatile uint8_t m_FirstPromptDC;
extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

PROTOCOL_SOLVER PROTOCOL_CAN_SOLVER = {
  ConnectToCANDevice,
  SendAndRecieveCANFrame,
  CheckTheResultsCANAndPerformAction
};


// definitions for structures ....

#define CAN_TX_MSG_CNT		25
#define CAN_RX_MSG_CNT		25


#define RECVD_CAN_NO		(CAN_1)

#define RECVD_CAN_CNTRL		(CAN1_CTRL)

//messages for test Acceptance Filter mode
__IO CAN_MSG_Type AFTxMsg[CAN_TX_MSG_CNT], AFRxMsg[CAN_RX_MSG_CNT];
__IO  uint32_t CANRxCount = 0, CANTxCount = 0, CANRxCountFlag = 0;

uint32_t CANErrCount = 0;
uint8_t IsCANCompletedOrNot = 0;
__IO  uint32_t Expected_CANTxCount = 0;


 
AF_SectionDef AFTable;
FullCAN_Entry FullCAN_Table[6];
SFF_Entry SFF_Table[6];
SFF_GPR_Entry SFF_GPR_Table[6];
EFF_Entry EFF_Table[6];
EFF_GPR_Entry EFF_GPR_Table[6];
///////////////////////////////////


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
* @brief		CAN IRQ Handler
* @param[in]	none
* @return 		none
**********************************************************************/
void CAN_IRQHandler(void)
{
  uint8_t IntStatus;
  
  //check FullCAN interrupt enable or not
  if(CAN_FullCANIntGetStatus()== SET)
  {
    //check is FullCAN interrupt occurs or not
    if ((CAN_FullCANPendGetStatus(FULLCAN_IC0))
        ||(CAN_FullCANPendGetStatus(FULLCAN_IC1)))
    {
      //read received FullCAN Object in Object Section
      FCAN_ReadObj((CAN_MSG_Type *)&AFRxMsg[CANRxCount]);
      CANRxCountFlag = 1;
      CANRxCount++;
    }
  }
  
  /* get interrupt status
  * Note that: Interrupt register CANICR will be reset after read.
  * So function "CAN_IntGetStatus" should be call only one time
  */
  IntStatus = CAN_IntGetStatus(RECVD_CAN_NO);
  
  //check receive interrupt
  if((IntStatus >> 0) & 0x01)
  {
    CAN_ReceiveMsg(RECVD_CAN_NO, (CAN_MSG_Type *)&AFRxMsg[CANRxCount]);
    CANRxCountFlag = 1;
    CANRxCount++;
  }
}

/*-------------------------PRIVATE FUNCTIONS----------------------------*/
/*********************************************************************//**
* @brief		Print Message via COM1
* param[in]	msg: point to CAN_MSG_Type object that will be printed
* @return 		none
**********************************************************************/
#ifndef RELEASE_CODE
void PrintMessage(CAN_MSG_Type* CAN_Msg)
{
  uint32_t data;
  
  _DBG("Message ID:     ");
  
  _DBH32(CAN_Msg->id);_DBG_("");
  
  _DBG("Message length: ");
  
  _DBH32(CAN_Msg->len);_DBG_(" BYTES");
  
  _DBG("Message type:   ");
  
  if(CAN_Msg->type==DATA_FRAME)
  {
    _DBG_("DATA FRAME ");
  }
  else
  {
    _DBG_("REMOTE FRAME ");
  }
  
  _DBG("Message format: ");
  
  if(CAN_Msg->format==STD_ID_FORMAT)
  {
    _DBG_("STANDARD ID FRAME FORMAT");
  }
  else
  {
    _DBG_("EXTENDED ID FRAME FORMAT");
  }
  
  _DBG("Message dataA:  ");
  
  data = (CAN_Msg->dataA[0])|(CAN_Msg->dataA[1]<<8)|(CAN_Msg->dataA[2]<<16)|(CAN_Msg->dataA[3]<<24);
  
  _DBH32(data);_DBG_("");
  
  data = (CAN_Msg->dataB[0])|(CAN_Msg->dataB[1]<<8)|(CAN_Msg->dataB[2]<<16)|(CAN_Msg->dataB[3]<<24);
  
  _DBG("Message dataB:  ");
  
  _DBH32(data);_DBG_("");
  
  _DBG_("");
}

#endif

void ConstructTheCANFrm(Can_Device * canDevice, uint8_t * buf, uint16_t sizeBuf, uint8_t containing_size)
{
	int frame_number = 0;
	int byte_size = 0;
	int i =0;
  Expected_CANTxCount = 0;
  
  
  if(sizeBuf > 7)
  {
    frame_number = sizeBuf / 7;
  }
  else
  {
    frame_number = 0;
  }
  
  
  
  byte_size = 0;
  
  if(frame_number == 0)
  {
    AFTxMsg[0].id = canDevice->ID_part;
    AFTxMsg[0].len = 0x08;
    AFTxMsg[0].type = DATA_FRAME;
    AFTxMsg[0].format = STD_ID_FORMAT;
    
    if((containing_size == 1))
      AFTxMsg[0].dataA[0] = sizeBuf;
    else
      AFTxMsg[0].dataA[0] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      
      AFTxMsg[0].dataA[1] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataA[2] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataA[3] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataB[0] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataB[1] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataB[2] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
      AFTxMsg[0].dataB[3] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest; 
      
      Expected_CANTxCount = 0;
  }
  else
  {
    for (i = 0 ; i <= frame_number ; i++)
    {
      AFTxMsg[i].id = canDevice->ID_part;
      AFTxMsg[i].len = 0x08;
      AFTxMsg[i].type = DATA_FRAME;
      AFTxMsg[i].format = STD_ID_FORMAT;
      
      if(i == 0)
      {
        AFTxMsg[i].dataA[0] = 0x10;
      }
      else
      {
        AFTxMsg[i].dataA[0] = 0x20+i;
      }
      
      if((i== 0))
        AFTxMsg[i].dataA[1] = sizeBuf;
      else
        AFTxMsg[i].dataA[1] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataA[2] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataA[3] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataB[0] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataB[1] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataB[2] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;
        AFTxMsg[i].dataB[3] = (byte_size++ < sizeBuf) ? *buf++ : canDevice->fill_the_Rest;   
    }
    Expected_CANTxCount = frame_number+1;
  }
}

char RunTheFilteringJustForOnce = 0;
uint8_t ConnectToCANDevice(DEVICE_TYPES * Spec)
{
  uint32_t i;
  volatile uint32_t cnt;
  CAN_ERROR error;
	Can_Device * can_spec = Spec->can_spect;
	
  //CAN_DeInit(CAN_1);
  
#ifndef RELEASE_CODE
  _DBG_("Initialize CAN ...");
#endif
  
	//open the relay :
	Activate_ProtocolPin(PROTOCOL_KWP_RELAY, OFF_MODE);
	Activate_ProtocolPin(PROTOCOL_TYPE1_RELAY, OFF_MODE);
	Activate_ProtocolPin(PROTOCOL_CAN_RELAY, ON_MODE);

	
	Activate_OBDPin(can_spec->high_pin, ON_TX_MODE);
	Activate_OBDPin(can_spec->low_pin, ON_RX_MODE);
  
	
	
  PINSEL_ConfigPin (0, 0, 1);
  
  PINSEL_ConfigPin (0, 1, 1);
  
  //Initialize CAN1 & CAN2
  CAN_Init(CAN_1, can_spec->baudrate);
  
  //Enable Receive Interrupt
  CAN_IRQCmd(RECVD_CAN_NO, CANINT_FCE, ENABLE);
  CAN_IRQCmd(RECVD_CAN_NO, CANINT_RIE, ENABLE);
  
  //Enable CAN Interrupt
  NVIC_EnableIRQ(CAN_IRQn);
#ifndef RELEASE_CODE
  _DBG_("Initialize AFTable ...");
#endif
  /* initialize AF Look-up Table sections*/
  //CAN_SetupAFTable();
  CAN_SetupAFTableArray(Spec->can_spect->filter_array);
  
  /* install AF Look-up Table */
  
  //if(RunTheFilteringJustForOnce == 0)
  {
    error = CAN_SetupAFLUT(&AFTable);
    RunTheFilteringJustForOnce =  1;
    if (error != CAN_OK)
    {
#ifndef RELEASE_CODE
      _DBG_("Setup AF: ERROR...");
#endif
      return 1; // AF Table has error
    }
  }

  
  //////////////////////////////////////////////////////////////
  
  return 0;
  
}


uint8_t CheckTheResultsCANAndPerformAction(uint8_t status)
{
	int res = 0;

  if(status == STATUS_ECU_CONNECTION_LOST /*status == STATUS_ECU_CONNECTION_7F || */)
  {
		/*
		taskDiagRAM tskDg;
    taskDiagRAM * tskPointer = &tskDg;
		
		m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect); 
    
    ConvertTask(task[CONNECT], tskPointer);
    
    res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
                                                                  tskPointer->frmFormat[0].frm, 
                                                                  tskPointer->frmFormat[0].frm_size,0);
	
*/
		int temp_cmd = m_Scheduler_Buffer.CMD;
		m_Scheduler_Buffer.CMD = CMD2_TASK_DATA_CONNECT;
		
		
		m_ConnectionStatus = CONNECTION_TO_CAR_NO;
		
		
		res = GeneralInitConnectProcedure(1);
		
		if(m_Scheduler_Buffer.CMD != CMD2_TASK_DATA_DISCONNECT)
			m_Scheduler_Buffer.CMD = temp_cmd;
		
		
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
	
				buffer_out[j++] = 0x60;
			
				WriteTheFrame(UART_1, CMD1_TASK_DATA, m_Scheduler_Buffer.CMD , buffer_out , 4);
				
				m_FirstPromptDC = 1;
			}
			

			
			
			//SendStatusToDevice();
			//delay_ms(1000);
			//now should reset:
			
			//#ifdef __RAM_MODE__
			//Reset_Arm(10);
			//#endif
		}
		else
		{
				m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
		    Activate_LEDPin(LED_CONNECT,ON_MODE);
		}
    

    return 1;
  }
  
  return 0;
}




/*********************************************************************//**
* @brief		Init AF-Look Up Table Sections entry value
* 				We setup entries for 5 sections:
* 				- 6 entries for FullCAN Frame Format Section
* 				- 6 entries for Explicit Standard ID Frame Format Section
* 				- 6 entries for Group of Standard ID Frame Format Section
* 				- 6 entries for Explicit Extended ID Frame Format Section
* 				- 6 entries for Group of Extended ID Frame Format Section
* @param[in]	none
* @return 		none
**********************************************************************/
void CAN_SetupAFTable(void)
{
  SFF_Table[0].controller = RECVD_CAN_CNTRL;
  SFF_Table[0].disable = MSG_ENABLE;
  SFF_Table[0].id_11 = 0x650;
  
  SFF_Table[1].controller = RECVD_CAN_CNTRL;
  SFF_Table[1].disable = MSG_ENABLE;
  SFF_Table[1].id_11 = 0x651;
  
  SFF_Table[2].controller = RECVD_CAN_CNTRL;
  SFF_Table[2].disable = MSG_ENABLE;
  SFF_Table[2].id_11 = 0x652;
  
  SFF_Table[3].controller = RECVD_CAN_CNTRL;
  SFF_Table[3].disable = MSG_ENABLE;
  SFF_Table[3].id_11 = 0x653;
  
  SFF_Table[4].controller = RECVD_CAN_CNTRL;
  SFF_Table[4].disable = MSG_ENABLE;
  SFF_Table[4].id_11 = 0x654;
  
  SFF_Table[5].controller = RECVD_CAN_CNTRL;
  SFF_Table[5].disable = MSG_ENABLE;
  SFF_Table[5].id_11 = 0x655;
  
  AFTable.SFF_Sec = &SFF_Table[0];
  AFTable.SFF_NumEntry = 6;
}


void CAN_SetupAFTableArray(CAN_FILTERING_ARRAY * can_filter)
{
	int i =0;
  for ( i = 0 ; i < can_filter->id_number ; i++)
  {
    SFF_Table[i].controller = RECVD_CAN_CNTRL;
    SFF_Table[i].disable = MSG_ENABLE;
    SFF_Table[i].id_11 = can_filter->array_ids[i];    
  }
  
  AFTable.SFF_Sec = &SFF_Table[0];
  AFTable.SFF_NumEntry = can_filter->id_number;
	AFTable.FC_NumEntry = 0;
	AFTable.EFF_GPR_NumEntry = 0;
	AFTable.EFF_NumEntry = 0;
	AFTable.SFF_GPR_NumEntry = 0;
  
}


void ResetCANCountRxTx(uint8_t total_rec_reset)
{
  if(total_rec_reset == 1)
    CANRxCount = 0;
  //CANRxCount = 0;
  CANTxCount = 0;
  CANRxCountFlag = 0;
  CANErrCount = 0;
  IsCANCompletedOrNot = 0;
  m_iFlagRecieveFrame = 0;
}




uint8_t SendAndRecieveCANFrame(DEVICE_TYPES * Dev,  uint8_t * send_buffer, uint16_t size_of_buffer, uint8_t bank_frm_num)
{
  int y1,y2,y4  = 0;
	int number = 0;
  int flag_ret = 1;
  Can_Device * canDev = Dev->can_spect;
  
  ResetCANCountRxTx(1);
  ConstructTheCANFrm(canDev,send_buffer, size_of_buffer,1);
  
  if(Expected_CANTxCount == 0)
  {
    CAN_SendMsg(CAN_1, (CAN_MSG_Type *)&AFTxMsg[0]);
  }
  else
  {
    CAN_SendMsg(CAN_1, (CAN_MSG_Type *)&AFTxMsg[0]);
		
    delay_ms_interrupt(canDev->TimeOut);
    while((!m_iFlagRecieveFrame) & (!CANRxCountFlag));
    Disable_Timer0();
    
    //analyze the recieved frame:
    if(AFRxMsg[0].dataA[0] == 0x30)
    {
      if(AFRxMsg[0].dataA[1] == 0x00 || AFRxMsg[0].dataA[1] >= Expected_CANTxCount)
      {
        for (y2 = 1 ; y2 < Expected_CANTxCount ; y2++)
        {
					delay_ms(50);
          CAN_SendMsg(CAN_1, (CAN_MSG_Type *)&AFTxMsg[y2]);
					
        }
      }
      else
      {
        int num_in_each_request = Expected_CANTxCount / AFRxMsg[0].dataA[1];
        int counter_1 = 1;
        
        do
        {
          
          for (y4 = 0 ; y4 < AFRxMsg[0].dataA[1] ; y4++)
          {
						delay_ms(50);
            CAN_SendMsg(CAN_1, (CAN_MSG_Type *)&AFTxMsg[counter_1++]);
          }
          
          delay_ms_interrupt(canDev->TimeOut);
          while((!m_iFlagRecieveFrame) & (!CANRxCountFlag));
          Disable_Timer0();
          
          counter_1++;
          
        } while(counter_1 < Expected_CANTxCount);
      }
    }
  }
  
  ResetCANCountRxTx(1);
  delay_ms_interrupt(canDev->TimeOut);
  while((!m_iFlagRecieveFrame) & (!CANRxCountFlag));
  Disable_Timer0();
	


	
  
  if(m_iFlagRecieveFrame == 1)
  {
#ifndef RELEASE_CODE
    _DBG_("CAN TIMEOUT ...");
#endif
    return STATUS_ECU_CONNECTION_LOST;
  }
  
  
  //number of the frame should be rea
  if(AFRxMsg[0].dataA[0] == 0x10)
  {
    int frame_num = AFRxMsg[0].dataA[1] / 7;
    int internal_frame_Send = 0;
    int num_frame_in_each_time = 0;
    int tr,r=0;
    
    if((canDev->split_part == 0) || (canDev->split_part == 0x96))
    { 
      internal_frame_Send = 1;
      num_frame_in_each_time = frame_num;
      
      if(canDev->split_part == 0)
      {
        uint8_t buf_rest[8] = {0x30,0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
				buf_rest[1] = num_frame_in_each_time;
				
        ConstructTheCANFrm(canDev,buf_rest, 7, 0);
      }
      else if (canDev->split_part == 0x96)
      {
        uint8_t buf_rest[] = {0x30,0x96,0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
        ConstructTheCANFrm(canDev,buf_rest, 8, 0);        
      }
      
    }
    else
    {
			uint8_t buf_rest[2] = {0x30,0};
			
      internal_frame_Send = frame_num / canDev->split_part;
      num_frame_in_each_time = canDev->split_part;
			buf_rest[1] = num_frame_in_each_time;
			
      
			
      ConstructTheCANFrm(canDev,buf_rest, 2, 0);
      
    }
    
    

    for (tr = 0 ; tr < internal_frame_Send ; tr++)
    {
      ResetCANCountRxTx(0);
      CAN_SendMsg(CAN_1, (CAN_MSG_Type *)&AFTxMsg[0]);
      delay_ms_interrupt(canDev->TimeOut);
      
      for (r = 0 ; r < num_frame_in_each_time ; r++)
      {
        while( (!m_iFlagRecieveFrame) & (!CANRxCountFlag));
        
        if(m_iFlagRecieveFrame == 1)
        {
#ifndef RELEASE_CODE
          _DBG_("CAN TIMEOUT ...");
#endif
          return STATUS_ECU_CONNECTION_LOST;
          break;
        }
        
        //PrintMessage((CAN_MSG_Type *)&AFRxMsg[CANRxCount ? 0 : CANRxCount-1]);_DBG_("");
        CANRxCountFlag = 0;
        //delay_ms(canDev->Delay_BetweenFrm);
      }
      
    }
    
    //fill the bank bufer:
    m_frmBank[bank_frm_num].rec_byte_number = AFRxMsg[0].dataA[1];
    
    for ( y1 = 0 ; y1 < CANRxCount ; y1++)
    {
      if(y1 != 0)
        m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataA[1];
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataA[2];
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataA[3];
      
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataB[0];
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataB[1];
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataB[2];
      m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[y1].dataB[3];
    }
    
		m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
    return STATUS_ECU_CONNECTION_OKAY;
		
    
  }
  else if (AFRxMsg[0].dataA[0] >= 0x2)
  {
    int number = 0;
    //m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataA[0];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataA[1];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataA[2];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataA[3];
    
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataB[0];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataB[1];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataB[2];
    m_frmBank[bank_frm_num].frm[number++] = AFRxMsg[0].dataB[3];
    
		
		m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
    return STATUS_ECU_CONNECTION_OKAY;
		
		/*
		
    if(AFRxMsg[0].dataA[1] == 0x7f)
    {
      return STATUS_ECU_CONNECTION_7F;
    }
    else
    {
			m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
      return STATUS_ECU_CONNECTION_OKAY;
			
    }
    */
  }
  
  m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
  return STATUS_ECU_CONNECTION_OKAY;
	
}

/*

		//
	
	if(m_iFlagRecieveFrame == 0)
	{
	while(AFRxMsg[0].dataA[1] == 0x7f && AFRxMsg[0].dataA[3] == 0x78)
	{
		ResetCANCountRxTx(1);
		delay_ms_interrupt(canDev->TimeOut);
		while((!m_iFlagRecieveFrame) & (!CANRxCountFlag));
		Disable_Timer0();	
			
		if(m_iFlagRecieveFrame == 1)
			break;
	}		
	}
*/