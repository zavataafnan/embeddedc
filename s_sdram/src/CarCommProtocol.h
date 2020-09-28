#pragma once
#include "globalDef.h"
#include "Protocol_CAN.h"
#include "SchedulerRunner.h"


#define STATUS_ECU_CONNECTION_OKAY 0x00
#define STATUS_ECU_CONNECTION_LOST 0x01
#define STATUS_ECU_CONNECTION_CONNECT 0x02
//#define STATUS_ECU_CONNECTION_7F 0x03


#define PROTOCOL_CAN 0x01
#define PROTOCOL_KWP 0x02
#define PROTOCOL_SL96 0x03


#define STATUS_COMM_ALIVE        0x00
#define STATUS_COMM_DISCONNECT   0x01
#define STATUS_COMM_NO_ECU       0x02


#define NUMBER_OF_FRAME_BUFFER 5

#define NUMBER_OF_BUFFER_FOR_OUTPUT 14

#define CONNECTION_TO_CAR_OKAY   1
#define CONNECTION_TO_CAR_NO     0


 

typedef struct _comm_status_
{
  uint8_t status;
} COMM_STATUS;

typedef struct _frame_bank_
{
  uint8_t frm[120];
  uint16_t rec_byte_number;
} m_FrmBank;

typedef struct _string_buffer_
{
  char buf[1][24];
  uint8_t enable_or_disable;
  uint8_t exceptional_condition;
  uint8_t exceptional_value;
  uint8_t should_be_updated;
		uint16_t item_num;
} STRING_BUFFER;




typedef struct _response_saver_for_scheduler_
{
  STRING_BUFFER task_number[24];
} SCHEDULER_BUFFER;



typedef struct _frame_analyzer_
{
  uint8_t frm[24];
  uint8_t frm_size;
  uint8_t delay_between_byte;
  uint16_t delay_between_frame;
 
} frameFormat;


//typedef struct _data_internal_
//{             
//  const int 
//} dataInternalStruct;

typedef struct _type_data_str_
{
  char * data;
} TYPE_STR_DATA;

typedef struct _type_data_
{
 const char * point_char_data;
 const char * point_char_data_1;
 const int * point_int_data;
 const int * point_int_data_1;
 const TYPE_STR_DATA * point_str_data;
 int size_of_data;
 
} TYPE_DATA_INTERNAL;



typedef struct _taskDiag_ 
{
  const uint8_t num_frame_send;
  const  frameFormat * frmFormat;
  void (*func_calculator) (char *,int);
  const uint8_t type_task;
  const int number_of_index;
  const int * index_array;
  const void * point_internal_data;
} taskDiag;

typedef struct _taskDiagRAM_ 
{
   uint8_t num_frame_send;
    frameFormat * frmFormat;
  void (*func_calculator) (char *,int);
   uint8_t type_task;
   int number_of_index;
   int * index_array;
   void * point_internal_data;
} taskDiagRAM;

typedef struct _scheduler_
{
  uint8_t type;
  char (*scheduler_func)( char *, int, int);
} SCHEDULER; 


typedef struct _PROTOCOL_SOLVER_ {
  uint8_t (*ConnectProcedure) ( DEVICE_TYPES * Spec);
  uint8_t (*SendAndRecieveFrame) ( DEVICE_TYPES * DevSpec, uint8_t * buffer, uint16_t size_of_buffer, uint8_t bank_frm_num);
  uint8_t (*CheckTheResultsAndPerformAction) (uint8_t status);
	uint8_t (*isReadyFrame) (uint8_t * tx, uint32_t tx_head);
} PROTOCOL_SOLVER;


typedef struct _comm_protocol_profile_
{
   uint8_t protocol_type;
   DEVICE_TYPES * spect;
   PROTOCOL_SOLVER * protocol_solver;
   const taskDiag * task;
} ProtocolProfiler;
            

