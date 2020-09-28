#pragma once
#include "globalDef.h"
#include "CarCommProtocol.h"

#define TASK_TYPE_CONNECT 0x00
#define TASK_TYPE_STANDBY 0x01
#define TASK_TYPE_PARAM 0x02

#define SUCCESS_FULL 0x00
#define NOT_OKAY_FRAME 0x01

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define SCHEDULER_TYPE_CONTINOUS 0x00
#define SCHEDULAR_TYPE_ONLY_ONCE 0x01


//#define TYPE_STANDBY    0x00
//#define TYPE_CONNECT    0x01
//#define TYPE_DISCONNECT 0x02
//#define TYPE_EMPTY      0x03
//
//#define TYPE_SUMMARY    0x02
//#define TYPE_IDEN       0x03
//#define TYPE_FAULTS     0x04
//#define TYPE_PARAM      0x05
//#define TYPE_ACTAUTOR   0x06
//#define TYPE_CONFIG     0x07



#define STOP_SCHEDULAR 0x00
#define RUN_SCHEDULAR  0x01

typedef struct _scheduler_buffer_
{
  int input_buffer[50];
  int num;
} SHEDULER_BUFFER;







uint8_t TaskRun(void);
uint8_t CreateSchedule(void);

