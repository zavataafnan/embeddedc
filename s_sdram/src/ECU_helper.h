#ifndef _ECU_HELPER_H_
#define _ECU_HELPER_H_

#include "CarCommProtocol.h"
#include "ECU_define.h"


//#define CMD2_TASK_DATA_PARAMETER 0x14
//#define CMD2_TASK_DATA_CONFIGURATION 0x16
//#define CMD2_TASK_DATA_REMOTE_LEARNING 0x19
//#define CMD2_TASK_DATA_FAULTS 0x13
//#define CMD2_TASK_DATA_DOWNLOAD 0x20
//#define CMD2_TASK_DATA_CATEGORY 0xcc
//#define CMD2_TASK_DATA_ZIR_ACTUATOR 0x21
//#define CMD2_TASK_DATA_ACTUATOR 0x15
//#define CMD2_TASK_DATA_SUMMARY 0x11
//#define CMD2_TASK_DATA_IDENTIFICATION 0x12

#define PARAM_INDEX 3
#define CONFIG_INDEX 4
#define REMOTE_LEARNING_INDEX 5
#define FAULTS_INDEX 6
#define DOWNLOAD_INDEX  7
#define CATEGORY_INDEX 8
#define ZIR_ACTUATOR_INDEX 9
#define ACTUATOR_INDEX 10
#define SUMMARY_INDEX 11
#define IDEN_INDEX 12

#define CMD2_TASK_DATA_ERASEFAULT 0x33
#define CMD2_TASK_DATA_GRAPH   0x34


typedef struct __CMD_STRUCTURE__
{
	char CMD_State;
	char m_cmd_1;
	char m_cmd_2;
	char m_cmd_3;
} CMD_STRUCTURE;


typedef struct __FAULT_RESOLVER__
{
	uint32_t fault;
	uint8_t status;
} FAULT_RESOLVER;

int RunTheSpecificTask(const taskDiag * tskD);
char FindSpicificInputInArray(int test_number, const taskDiag task);
int FindTaskIndexForInput(int indx);
int checkSpecificDataInArray(int data, int * arrayData, int num);
uint8_t ConvertTask(const taskDiag tsk1, taskDiagRAM * tskDesc);
char SendRecievedTheRequiredFrames(char * input,int num); 
int checkSpecificDataInCharArray(int data, int * arrayData, int num);
void ResetTheBuffer(void);
void SetTheECUToSpecificState(int num);
void GeneralActProcedure(void);
void Runner(void);
void GeneralConnectProcedure(void);
void GeneralStandbyProcedure(void);
uint8_t reseTheMainBuffer(void);
uint8_t check_two_buffer(uint8_t * dest, uint8_t * src, uint32_t size_e);
int checkSpecificDataInArrayFromFlash(int data, const int * arrayData, int num);
void GeneralDisconnectProcedure(char change_status);
char FindTheRequredConfigAndSendAndRecieve(char * input,int num);
int FindTaskIndexForGivenStateInput(int indx, int cmd_type);
uint8_t GeneralInitConnectProcedure(char counter);
void GeneralStandbyWithOutDelay(void);
void SendFaultFrame(uint8_t * buf, int size_buf, int number_of_fault);
char SendRecievedTheRequiredFramesForGraph(char * input,int num);
void SendGeneralFrame(uint8_t * buf, int size_buf, int cmd1, int cmd2, int status1, int status2);
#endif
