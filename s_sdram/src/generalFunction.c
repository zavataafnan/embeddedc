#include "generalFunction.h"
#include "SchedulerRunner.h"
#include "CommProtocol.h"
#include "lpc177x_8x_wwdt.h"
#include "RelayController.h"
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
#define WDT_TIMEOUT 50

void Reset_Arm(uint32_t value)
{
	WWDT_Init(value);

	WWDT_Enable(ENABLE);

	WWDT_SetMode(WWDT_RESET_MODE, ENABLE);
	
	// Start watchdog with timeout given
	WWDT_Start(value);

	WWDT_Feed();

	//infinite loop to wait chip reset from WDT
	while (1);
}

void Buzzer_On(int time)
{
	Activate_LEDPin(LED_BUZZER,ON_MODE);
	delay_ms(time);
	Activate_LEDPin(LED_BUZZER,OFF_MODE);
	delay_ms(100);
}


char checkIdInArray(int idx)
{
	char res = 0;
	int i;
	
	//construct the inputs :
	int input_items[14];
	int index_cre =0;
	
	for (i = 0 ; i < m_Scheduler_Buffer.number_of_rec_byte /2 ;i++)
	{
		input_items[i] = makeint(m_Scheduler_Buffer.input_buf[index_cre++], m_Scheduler_Buffer.input_buf[index_cre++]);
	}
	
	

  for (i = 0 ; i < m_Scheduler_Buffer.number_of_rec_byte / 2; i++)
  {
    if(input_items[i] == idx)
    {
      res = 1;
      break;
    }
  }
  
  return res;
}

void memcpy_P(const uint8_t * src, uint8_t * dest , int num)
{
	int i=0;
  for (i = 0; i < num ; i++)
  {
    dest[i] = src[i];
  }
}

//parameteres .....
char CheckInFrme(int * input, int tskId, char tskNum)
{
  char flag = 0;
	int i=0;
  for (i = 0 ; i < tskNum ; i++)
  {
    if(input[i] == tskId)
    {
      flag = 1;
      break;
    }
  }
  
  return flag;
}

char checkbiter(char address,char bit)
{
  char temp = (address &=(1<<bit));
  
  if(temp > 0)
    return (1);
  else
    return (0);
}


unsigned int makeint(char high,char low)
{
  return(high*0x100 + low);
}

signed int makesignint(char high,char low)
{
  if((high*0x100 + low) > 0x7FFF){
		return -(0xFFFF - (high*0x100 + low));
	}
	else
	  return(high*0x100 + low);
}
signed char makesignchar(char Input)
{
  if(Input > 0x7F){
    return -(0xFF - Input);
  }
  else
    return Input;
}

void array_copy( char copy[],char c_start,char c_end,char main[],char m_start,char m_end)
{
  char c_counter;
  char m_counter = m_start;
  for(c_counter=c_start;c_counter<c_end+1;c_counter++)
  {
    copy[c_counter] = main[m_counter];
    m_counter++;
  }
}

void HexConvert(char * buf_hex, char * buf, char num)
{  
	int kkk=0;
  for (kkk = 0 ; kkk < num ; kkk++)
  {
    sprintf(buf+kkk*2, "%02X", buf_hex[kkk]);
  }
  
  buf[num*2+1] = 0;
}

