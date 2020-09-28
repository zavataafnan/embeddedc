
#include "CommProtocol.h"
#include "lpc177x_8x_rtc.h"

#include "debug_frmwrk.h"
#include "analyzeDateTime.h"
#include "CarCommProtocol.h"
#include "SchedulerRunner.h"
#include "ECU_definition.h"
#include "uart_helper.h"
#include "CRCGenerator.h"
#include "ECU_define.h"
#include "spi_helper.h"
#include "securityLevels.h"
#include "EEPROMManage.h"
#include "ECU_helper.h"
#include <stdlib.h>
#include "version.h"
#include "lpc177x_8x_eeprom.h"
#include "b64.h"
#include "lib_crc.h"
#include "GeneralFunction.h"
#include "RelayController.h"
//m_FrameTotalStructure m_frmTotalStruct;
//COMM_STATUS m_CommStatus;
extern SECURITY_ID m_SecurityID;
SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
//extern ProtocolProfiler profile1;
//extern ProtocolProfiler * m_CurrentProfile;
//extern SCHEDULER * m_SchedulerArrayPointer;
//extern STRING_BUFFER scheduler_output_buffer_comparator[24];
//SCHEDULER * m_SchedulerPointer;
//m_FrameSettingCmdStructure m_frmSettingCmd = {0x10, 0x20, 0x00, 0x1};
//extern ProtocolProfiler * m_CurrentProf;
uint8_t m_jump_Condition = 0;
const m_FrameSettingCmdStructure m_frmSettingCmd = {0x10, 0x20, 0x00, 0x1};
extern int m_SecurityStatus;
extern int m_counter_fault;
extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
unsigned char m_State_Recieieved = 0;
extern SECURITY_ID m_SecurityID;
extern volatile uint8_t m_ConnectionStatus;
uint8_t m_disableBufferStatus = 0;
extern volatile uint8_t m_startProcess;
extern CMD_STRUCTURE m_CmdStruct;
extern volatile uint8_t m_standbyTermination;


void WriteTheFrame(UART_ID_Type UartID, uint8_t cmd1 , uint8_t cmd2, uint8_t* pyFrame, int nNumByte)
{
    int result = 0x8409;
	int i,t = 0;
	uint16_t buf_cmd_size;
	unsigned char * frm;
	unsigned char * buf_cmd;
	int size_buf = 0;
	
  m_FrameCmdStructure m_frmHeaderCmd = {    
    0x80, //ucStart
    sizeof(m_FrameCmdStructure),  //m_iLen
    0x11, //cmd1
    0xff //cmd2
  };
  
  m_frmHeaderCmd.m_ucCmd1 = cmd1;
  m_frmHeaderCmd.m_ucCmd2 = cmd2;
  
  buf_cmd = (unsigned char *)(&(m_frmHeaderCmd));
  buf_cmd_size = sizeof(m_FrameCmdStructure);
  
  m_frmHeaderCmd.m_iLen = nNumByte + buf_cmd_size+1  + 2;
  
  frm = (unsigned char *)malloc(m_frmHeaderCmd.m_iLen);
  
  memcpy(frm, buf_cmd, sizeof(m_frmHeaderCmd));
  memcpy(frm + sizeof(m_FrameCmdStructure)+1, pyFrame, nNumByte);
  
  frm[sizeof(m_frmSettingCmd)] = 0;
  
  for (i = 0 ; i < sizeof(m_frmSettingCmd) ; i++)
  {
    t += (unsigned char)frm[i];
  }
  frm[sizeof(m_FrameCmdStructure)] = t;  
  
	
	    do {

        unsigned char val_t = (unsigned char)frm[size_buf];
			  //sprintf(buffera, "%X,%d \r\n", crc_ccitt_0000, val_t);
				//_DBG(buffera);
        result = crcByte(result, val_t);
        size_buf++;

    } while (size_buf < m_frmHeaderCmd.m_iLen - 2);


		
		
	/*
  if(checkbiter(m_frmSettingCmd.m_ucCRCType, FLAG_CRC_POLY_CRCCCITT))
  {
    CRC_Init(CRC_POLY_CRCCCITT);
    
#ifndef RELEASE_CODE
    //_DBG("CRC-CCITT Result: ");
#endif
    result = CRC_CalcBlockChecksum(frm, m_frmHeaderCmd.m_iLen-2, CRC_WR_8BIT);
    
#ifndef RELEASE_CODE    
    //_DBH32(result);_DBG_("");
#endif
  }
	*/
  
  
  frm[m_frmHeaderCmd.m_iLen-2] = result & 0xff;
  frm[m_frmHeaderCmd.m_iLen-1] = (result >> 8) & 0xff; 
  
  Write_ModeBusFrame(UartID, frm, m_frmHeaderCmd.m_iLen);
  
  free(frm);
  
  
}


/******************************************************
*** check that the valid frame is recieved or not   ***
******************************************************/
unsigned char isReadyFrame(uint8_t * tx, uint32_t tx_head)
{
  
  m_FrameCmdStructure * frmStr = (m_FrameCmdStructure *)tx;
  if(frmStr->m_iLen == tx_head)
  {
    return 1;
  }
  
  return 0;
}




unsigned char AnalyzeFrame(uint8_t * tx, uint32_t tx_head)
{
  char temp_var[7];
  char crc_cmd;
  m_FrameCmdStructure * frmCmdStruct;

  uint8_t * buf_data;
	uint16_t size_of_buffer;
	
	crc_cmd = 0;
	temp_var[6] = 0;
	memcpy(temp_var, tx, 7);
  frmCmdStruct = (m_FrameCmdStructure *)temp_var;
  crc_cmd = temp_var[6];
	
  size_of_buffer = frmCmdStruct->m_iLen - sizeof(m_FrameCmdStructure) - 3;
  //buf_data = (uint8_t *)malloc(size_of_buffer);
  //memcpy(buf_data, tx+ sizeof(m_FrameCmdStructure) + 1 , size_of_buffer);
  buf_data = tx+ sizeof(m_FrameCmdStructure) + 1;
  
  switch (frmCmdStruct->m_ucCmd1)
  {
    /*
  case CMD1_SECURITY:
    break;
    
  case CMD1_CHANGE_TIME:
    
    switch (frmCmdStruct->m_ucCmd2)
    {
    case CMD2_CHANGE_DATE_ALL:
      AnalyzeFrameChangeTime(buf_data);
      break;
      
    case CMD2_CHANGE_DATE_TIME:
      break;
      
    case CMD2_READ_DATE_TIME:
      AnalyzeFrameReadDateTime();
      break;
      
    }
    
    
    break;
    */
    
  case CMD1_TASK_DATA:
    AnalyzeFrameTaskStart(frmCmdStruct, buf_data);
    break;
    
  case CMD1_TASK_SECURITY:
    AnalyzeFrameSecurityTaskStart(frmCmdStruct, buf_data);
    break;
	
	case CMD1_TASK_LOG:
		
			//case CMD2_TASK_DATA_LOG:
			//send the log file :
		#ifdef LOGGING_SECTION
				SendLog();
	#endif
		  //WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_ACTUATOR , ret_frame , sizeof(ret_frame));
			//break;
		break;
	
	case CMD1_TASK_SCOPE:
		break;
	
	case CMD1_TASK_STATUS:
		AnalyzeStatusTask(frmCmdStruct, buf_data);
		break;
    
   
  }
  
  //free(buf_data);
  return 0;
}


/******************************************************
*** ruct the buffer frame ... ... ... .. ...   ***
******************************************************/
/*
0x80, addr, xor1, xor2, l1, l2, enc_type, frame_data, cs1, cs2 , (CCIT)!
or check sum!!!

*/


void AnalyzeFrameTaskStart(m_FrameCmdStructure * cmdstruct, uint8_t * buf)
{
  //find the ECU number : 
  int_char_little conv;
  uint16_t ecu_number = conv.i16;

  m_Scheduler_Buffer.CMD = cmdstruct->m_ucCmd2;
	m_startProcess = 0;
  m_Scheduler_Buffer.first_entering = 1;
  m_Scheduler_Buffer.number_of_rec_byte = cmdstruct->m_iLen - sizeof(m_FrameCmdStructure) - 2-2;
  memcpy(m_Scheduler_Buffer.input_buf,buf, m_Scheduler_Buffer.number_of_rec_byte);
  m_disableBufferStatus = 1;
	m_standbyTermination = 1;
  //m_Scheduler_Buffer
  reseTheMainBuffer();
	
	m_Scheduler_Buffer.send_to_debug = 1;
  switch (cmdstruct->m_ucCmd2)
  {
		
		#ifdef __RAM_MODE__
		
		case CMD2_TASK_DATA_CONNECT:
			
		{
		JUMP_CONNECTION tempC;
		conv.ai8[0] = buf[1];
		conv.ai8[1] = buf[0];
		ecu_number =  conv.i16;
		//m_Scheduler_Buffer.ecu_number = conv.i16;
		
					
			tempC.m_ConnectCondition = 1;
			tempC.m_ECU_requested = ecu_number;
			tempC.m_StatusCondition = 0;
		  
			EEPROM_Write(0,0x20,(void*)&tempC,MODE_8_BIT,sizeof(JUMP_CONNECTION));
			//EEPROM_Write(PAGE_OFFSET,PAGE_ADDR,(void*)buffer,MODE_8_BIT,output_file);
			//EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)crypted_buf,MODE_8_BIT,exact_8_18_size);
			//char ret_frame[] = { 0x11, 0};
			//WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_CONNECT , ret_frame , sizeof(ret_frame));
		
			Reset_Arm(0);
		}
		
		
		/*
		if(ecu_number == ECU_NUMBER)
		{
		
		}
		else
		{

			JUMP_CONNECTION tempC;
			tempC.m_ConnectCondition = 1;
			tempC.m_ECU_requested = ecu_number;
			tempC.m_StatusCondition = 0;
		  
			EEPROM_Write(0,0x20,(void*)&tempC,MODE_8_BIT,sizeof(JUMP_CONNECTION));
			//EEPROM_Write(PAGE_OFFSET,PAGE_ADDR,(void*)buffer,MODE_8_BIT,output_file);
			//EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)crypted_buf,MODE_8_BIT,exact_8_18_size);
			//char ret_frame[] = { 0x11, 0};
			//WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_CONNECT , ret_frame , sizeof(ret_frame));
		
			Reset_Arm(0);
		}
		*/
		#endif


		break;
		

		case CMD2_TASK_DATA_FAULTS:
			m_counter_fault = 0;
		break;
		
  case CMD2_TASK_DATA_SCOPE_DATA:
	case CMD2_TASK_DATA_SCOPE_VOLTAGE:
      m_State_Recieieved = 1;   
    break;
	
	case CMD2_TASK_DATA_ACTUATOR:
	//CMD_STRUCTURE m_CmdStruct;
	//consider that the command is helper :
	m_Scheduler_Buffer.send_to_debug =1;
	
	
	if(m_CmdStruct.CMD_State == 1)
	{
		m_CmdStruct.m_cmd_1 = m_Scheduler_Buffer.input_buf[2];
		m_Scheduler_Buffer.CMD = 0xff;
		
	}
	
	if(m_Scheduler_Buffer.input_buf[2] == 0x1)
	{
		m_CmdStruct.CMD_State = 1;
		m_CmdStruct.m_cmd_1 = 0;
		m_CmdStruct.m_cmd_2 = 0;
		m_CmdStruct.m_cmd_3 = 0;
		
	}
	
	
	break;
  
}
	}


void SendingNotOkay(void)
{
  char buffer[] = {"NOTOKAY"};
  WriteTheFrame(UART_1, CMD1_TASK_SECURITY, CMD2_SECURITY_READ_DATA , buffer , sizeof(buffer));
}

void AnalyzeFrameSecurityTaskStart(m_FrameCmdStructure * cmdstruct, uint8_t * buf)
{
  //find the ECU number : 
	int i=0;
  int_char_little conv;
  uint16_t ecu_number = conv.i16;
	conv.ai8[0] = buf[0];
	conv.ai8[1] = buf[1];
		m_Scheduler_Buffer.CMD = cmdstruct->m_ucCmd2;
  m_Scheduler_Buffer.first_entering = 1;
  m_Scheduler_Buffer.number_of_rec_byte = cmdstruct->m_iLen - sizeof(m_FrameCmdStructure) - 2-1;
  memcpy(m_Scheduler_Buffer.input_buf,buf, m_Scheduler_Buffer.number_of_rec_byte);
  m_disableBufferStatus = 1;
  //m_Scheduler_Buffer
  
  switch (cmdstruct->m_ucCmd2)
  {
    
  case CMD2_SECURITY_READ_DATA:
    if(m_SecurityStatus == LEVEL_FIRST_ACCESS)
      WriteTheFrame(UART_1, CMD1_TASK_SECURITY, CMD2_SECURITY_READ_DATA , (char *) &m_SecurityID , sizeof(SECURITY_ID));
    else
    {
      SendingNotOkay();
    }
    break;
    
    
  case CMD2_TASK_DATA_SCOPE_DATA:
		case CMD2_TASK_DATA_SCOPE_VOLTAGE:
    //
      m_State_Recieieved = 1;
          
    break;
		
			case CMD2_TASK_DATA_ACTUATOR:
	//CMD_STRUCTURE m_CmdStruct;
	//consider that the command is helper :
	//if(m_CmdStruct
	if(m_CmdStruct.CMD_State == 1)
	{
		m_CmdStruct.m_cmd_1 = m_Scheduler_Buffer.input_buf[2];
		m_Scheduler_Buffer.CMD = 0xff;
		
	}
	
	if(m_Scheduler_Buffer.input_buf[2] == 0x1)
	{
		m_CmdStruct.CMD_State = 1;
		m_CmdStruct.m_cmd_1 = 0;
		m_CmdStruct.m_cmd_2 = 0;
		m_CmdStruct.m_cmd_3 = 0;
		
	}
	break;
	
  }
  
}


void AnalyzeStatusTask(m_FrameCmdStructure * cmdstruct, uint8_t * buf)
{
	int i=0;

   if(cmdstruct->m_ucCmd2 != 0x72)
		m_Scheduler_Buffer.CMD = cmdstruct->m_ucCmd2;
  m_Scheduler_Buffer.first_entering = 1;
  m_Scheduler_Buffer.number_of_rec_byte = cmdstruct->m_iLen - sizeof(m_FrameCmdStructure) - 2-1;
  memcpy(m_Scheduler_Buffer.input_buf,buf, m_Scheduler_Buffer.number_of_rec_byte);
  m_disableBufferStatus = 1;
  //m_Scheduler_Buffer
  
  switch (cmdstruct->m_ucCmd2)
  {
    
  case CMD2_TASK_DATA_STATUS:
	{
		
		//Initialize_Timer3(5500);
    //SendStatusToDevice();
		
			JUMP_CONNECTION tempC;
			tempC.m_ConnectCondition = 0;
			tempC.m_ECU_requested = 0xffff;
			tempC.m_StatusCondition = 1;
			EEPROM_Write(0,0x20,(void*)&tempC,MODE_8_BIT,sizeof(JUMP_CONNECTION));
		
		  Reset_Arm(0);
	}
	
    break;
	case CMD2_TASK_DATA_STATUS2:
		SendStatusToDevice();
		break;    
	
  }
  
}

void SendStatusToDevice(void)
{
	unsigned char send_frame[35];
	char * test_buf;
	int size_string = 0;
	spi1_send_frame(XVBAT, 0, 0);
	spi1_recieve_frame();

	send_frame[0] = xmegarx_frame[3];
	send_frame[1] = xmegarx_frame[4];
	send_frame[2] = m_SecurityID.access_level;
	send_frame[3] = m_SecurityID.reasons_of_blocks;
	send_frame[4] = m_ConnectionStatus;



	test_buf = b64_encode(m_SecurityID.m_SerialNumber, strlen(m_SecurityID.m_SerialNumber) + 1);
	size_string = strlen(test_buf);
	memcpy(send_frame + 5, test_buf, size_string);
	//test_buf_ref = b64_decode (test_buf, strlen(test_buf));

	free(test_buf);
	

	WriteTheFrame(UART_1, CMD1_TASK_STATUS, CMD2_TASK_DATA_STATUS, (char *)send_frame, size_string + 5);
}


