#ifndef __COMM_PROTOCOTOL_H__
#define __COMM_PROTOCOTOL_H__

#include "globalDef.h"
#include "lpc177x_8x_crc.h"
#include "lpc177x_8x_uart.h"
#include "CarCommProtocol.h"


#define setbit(address,bit) (address |= (1<<bit))
#define clearbit(address,bit) (address &= ~(1<<bit))

#define CMD1_SECURITY 0x10

#define CMD2_SECURITY_UNLOCK_LEVEL1  0x10
#define CMD2_SECURITY_UNLOCK_LEVEL2  0x11
#define CMD2_SECURITY_UNLOCK_LEVEL3  0x12
#define CMD2_SECURITY_UNLOCK_LEVEL4  0x13
#define CMD2_SECURITY_UNLOCK_LEVEL5  0x14
#define CMD2_SECURITY_UNLOCK_LEVEL6  0x15
#define CMD2_SECURITY_READ_DATA      0x25
#define CMD2_SECURITY_WRITE_SERIAL      0x26

#define CMD1_CHANGE_TIME                0x11
#define CMD2_CHANGE_DATE_ALL            0x10
#define CMD2_CHANGE_DATE_TIME           0x11
#define CMD2_READ_DATE_TIME             0x12
  
#define CMD1_TASK_DATA            0x12
#define CMD1_TASK_SECURITY        0x13
#define CMD1_TASK_LOG             0x14
#define CMD1_TASK_SCOPE           0x15
#define CMD1_TASK_STATUS          0x16

#define TASK_STANDBY_INDEX  0x00
#define TASK_CONNECT_INDEX  0x01
#define TASK_DISCONNECT_INDEX 0x02

#define CMD2_TASK_DATA_SCOPE_DATA 0x70
#define  CMD2_TASK_DATA_SCOPE_VOLTAGE 0x72
#define CMD2_TASK_DATA_STATUS   0x71
#define CMD2_TASK_DATA_STATUS2  0x72


#define CMD2_TASK_DATA_LOG  0x73


//
//
//#define CMD2_TASK_DATA_SUMMARY          0x11
//#define CMD2_TASK_DATA_IDEN             0x12
//#define CMD2_TASK_DATA_FAULT           0x13
//#define CMD2_TASK_DATA_PARAM       0x14
//#define CMD2_TASK_DATA_ACTUATOR         0x15
//#define CMD2_TASK_DATA_CONFIG    0x16
//


typedef struct _secruty_struct_
{
  uint8_t m_ucSecurityState;
} m_SecurityStructure;


typedef struct _frame_data_
{
	uint8_t m_ucSecond;
	uint8_t m_ucMinutes;
	uint8_t m_ucHour;
	uint8_t m_ucMonth;
	uint16_t m_iYear;
	uint8_t m_ucDayOfMonth;

} m_DateFrame;


typedef struct __TIME_DATE_ {
    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int second;
} TIME_DATE;


typedef struct _frame_buf_
{
  uint32_t m_iSizeOfBuffer;
  uint8_t * m_cBuffer;
} m_FrameBufferStructure;



typedef struct _frame_acr_
{
    uint8_t m_ucStart;
    uint16_t m_iLen;
    uint8_t m_ucCmd1;
    uint8_t m_ucCmd2;
} m_FrameCmdStructure;


typedef struct _setting_frame_acr_
{
	uint8_t m_XOR1;
	uint16_t m_XOR2;
	uint8_t m_ucEnc;
	uint8_t m_ucCRCType;
} m_FrameSettingCmdStructure;


typedef struct _frame_total_
{
	m_FrameCmdStructure m_frmCmd;
	uint32_t m_iSizeOfBuffer;
	uint8_t * m_cBuffer;
	//m_FrameBufferStructure m_frmBuf;
} m_FrameTotalStructure;


typedef struct _input_frame_
{
  char input_buf[4096];
  int number_of_rec_byte;
  int Scheduler_times;
  int Scheduler_running_time_fire;
  int New_Scheduler_Runner;
  char first_entering;
  int CMD;
	int new_CMD;
  const taskDiag * tskDiag;
	uint8_t send_to_debug;
} SCHEDULER_INPUT_MANAGEMENT;

typedef struct _data_struct_
{
  char char_format;
  int int_format;
  char array_format[10];
  char str_format[24];
} DATA_BUFFER;


/******************************************************
*** check that the valid frame is recieved or not   ***
******************************************************/
unsigned char isReadyFrame(uint8_t * tx, uint32_t tx_head);
unsigned char ConstructTheFrame(uint8_t * ret_buf , uint8_t * buf, uint32_t buf_size);
unsigned char EnterSecurityLevel(uint8_t security_level);
unsigned char UnlockToSpecificLevel(uint8_t * buf);
unsigned char AnalyzeFrame(uint8_t * tx, uint32_t tx_head);
char checkbiter(char address,char bit);
void AnalyzeFrameTaskStart(m_FrameCmdStructure * cmdstruct, uint8_t * buf);
void WriteTheFrame(UART_ID_Type UartID, uint8_t cmd1 , uint8_t cmd2, uint8_t* pyFrame, int nNumByte);
void AnalyzeFrameSecurityTaskStart(m_FrameCmdStructure * cmdstruct, uint8_t * buf);
unsigned char isReadyFrame(uint8_t * tx, uint32_t tx_head);
void AnalyzeStatusTask(m_FrameCmdStructure * cmdstruct, uint8_t * buf);
void SendStatusToDevice(void);
#endif
