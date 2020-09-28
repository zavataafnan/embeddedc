#ifndef __ECU_GLOBAL__
#define __ECU_GLOBAL__

//#include "ECU.h"
#include "generalFunction.h"

#include "globalDef.h"
#include "CommProtocol.h"
#include "Protocol_SL96.h"
#include "SchedulerRunner.h"

extern m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
extern STRING_BUFFER scheduler_output_buffer[NUMBER_OF_BUFFER_FOR_OUTPUT];
extern PROTOCOL_SOLVER PROTOCOL_CAN_SOLVER;
extern const taskDiag task[];

extern PROTOCOL_SOLVER PROTOCOL_KWP_SOLVER;
//5e
LONG_INITIALIZE long_init = {0x5e /*initialize byte*/
														, {0x55, 0x3b, 0x86} /*three bytes */
														, L_LINE_ON /* l-line executor */
														, ECU_ABS /* ECU TYPE */
														, 15
};
	
SHORT_INITIALIZE short_init = {TYPE_CONNECTION_SHORT, L_LINE_ON, 15};

SERIAL_PORT_CONFIG spc = {10400, /*baud rate*/ 
													NONE_PARITY,/*parity*/ 
													750};/*timeout*/

PIN_CONFIGURATION pi_con = {11, /*rec pin*/
														11, /*txd pin*/
														0, /*xor rxd*/
														1  /*xor txd*/
														};

Kwp_Device kwp_dev = {
											KWP_TYPE_IMMO, /*type of protocol*/ 
											INIT_SHORT_BIT, /* init both variaty */
											INIT_SHORT_TYPE,/*type of initialize*/
											{0xc0,0xf1},
											&short_init, /*if short initialize*/
										  &long_init, /*if long initialize*/
											&spc, /* serial port configuration */
											&pi_con, /* pin configuration*/			
}; 



DEVICE_TYPES device_type_E_20063 = {NULL, &kwp_dev};
//change here device -> &device...
ProtocolProfiler profile_E_20063 = {PROTOCOL_KWP,
																		&device_type_E_20063,
																		&PROTOCOL_KWP_SOLVER,
																		task};

char index_scheduler = 0;
char index_buffer_j = 0;
 ProtocolProfiler * m_CurrentProf = &profile_E_20063; 
 //SCHEDULER * m_SchedulerArrayPointer =  scheduler;

#endif
