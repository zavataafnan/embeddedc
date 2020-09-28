#include "log_Helper.h"
#include "uart_helper.h"
#include "string.h"



#ifdef LOGGING_SECTION
uint8_t m_LogingTemp[64000];
uint16_t m_SectionNum = 0;
uint16_t m_LogByteNum = 0;
uint8_t m_LogEnabled = 1;



void SaveTheFrameToLog(uint8_t * buf, uint16_t size_buf)
{
	uint16_t start_frame;
	uint16_t i =0;
	
	if((m_LogByteNum + size_buf) > 64000)
		m_LogByteNum = 0;
	for ( i = 0 ; i < size_buf ; i++)
	{
		m_LogingTemp[i + m_LogByteNum]  = buf[i];
	}
	
	m_LogByteNum+=size_buf;
	
	
}

void SendLog()
{
	Write_ModeBusFrame(UART_0, m_LogingTemp, (int)m_LogByteNum);
	memset(m_LogingTemp,0, m_LogByteNum);
	m_LogByteNum=0;
}

#endif