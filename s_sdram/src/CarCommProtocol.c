#include "CarCommProtocol.h"
#include "debug_frmwrk.h"
#include "globalDef.h"
#include "Protocol_CAN.h"
#include "SchedulerRunner.h"

m_FrmBank m_frmBank[NUMBER_OF_FRAME_BUFFER];
STRING_BUFFER scheduler_output_buffer[NUMBER_OF_BUFFER_FOR_OUTPUT];
STRING_BUFFER scheduler_output_buffer_final[NUMBER_OF_BUFFER_FOR_OUTPUT];
STRING_BUFFER scheduler_output_buffer_comparator[NUMBER_OF_BUFFER_FOR_OUTPUT];
volatile uint8_t m_ConnectionStatus = 0;
volatile uint8_t m_FirstPromptDC = 0;
//STRING_BUFFER scheduler_output_buffer_comparator[24];
//STRING_BUFFER scheduler_output_buffer_total;
//SCHEDULER_BUFFER m_GlobalShedulerBuffer;

ProtocolProfiler * m_CurrentProfile;

