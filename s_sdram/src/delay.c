#include "delay.h"
#include "lpc177x_8x_timer.h"
#include "debug_frmwrk.h"
#include "CommProtocol.h"
#include "ECU_define.h"

//extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

#define BRD_TIMER_USED				(LPC_TIM0)
#define BRD_TIM_INTR_USED			(TIMER0_IRQn)


volatile uint8_t m_iFlagRecieveFrame = 0;
volatile uint8_t m_iFlagRecieveUARTFrame;
volatile uint8_t m_standbyTermination =0 ;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
* @brief		TIMER0 interrupt handler sub-routine
* @param[in]	None
* @return 		None
**********************************************************************/
void TIMER0_IRQHandler(void)
{
  if (TIM_GetIntStatus(BRD_TIMER_USED, TIM_MR0_INT)== SET)
  {
    
    //_DBG_("Match interrupt occur...");
    m_iFlagRecieveFrame = 1;
    m_iFlagRecieveUARTFrame = 1;
  }
  
  
  TIM_ClearIntPending(BRD_TIMER_USED, TIM_MR0_INT);
}

void delay_ms_standby(uint32_t value)
{
  
	  TIM_TIMERCFG_Type TIM_ConfigStruct;
  TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	
  if(value == 0)
    return ;
  //timer init

  value = value * 1000;
  
  TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_ConfigStruct.PrescaleValue	= 1;
  
  // use channel 0, MR0
  TIM_MatchConfigStruct.MatchChannel = 2;
  // Disable interrupt when MR0 matches the value in TC register
  TIM_MatchConfigStruct.IntOnMatch   = TRUE;
  //Enable reset on MR0: TIMER will reset if MR0 matches it
  TIM_MatchConfigStruct.ResetOnMatch = TRUE;
  //Stop on MR0 if MR0 matches it
  TIM_MatchConfigStruct.StopOnMatch  = TRUE;
  //Toggle MR0.0 pin if MR0 matches it
  TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
  // Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10Hz)
  TIM_MatchConfigStruct.MatchValue   = value;
  
  // Set configuration for Tim_config and Tim_MatchConfig
  TIM_Init(LPC_TIM2, TIM_TIMER_MODE,&TIM_ConfigStruct);
  TIM_ConfigMatch(LPC_TIM2,&TIM_MatchConfigStruct);
  TIM_Cmd(LPC_TIM2,ENABLE);
  while ( !(TIM_GetIntStatus(LPC_TIM2,TIM_MR2_INT)) && (m_Scheduler_Buffer.CMD == CMD2_TASK_DATA_STANDBY));
  TIM_ClearIntPending(LPC_TIM2,TIM_MR2_INT);
  TIM_Cmd(LPC_TIM2,DISABLE);
}

void delay_ms(uint32_t value)
{
  
  //timer init
  TIM_TIMERCFG_Type TIM_ConfigStruct;
  TIM_MATCHCFG_Type TIM_MatchConfigStruct;
  value = value * 1000;
	
	if(value == 0)
		return;
  
  TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_ConfigStruct.PrescaleValue	= 1;
  
  // use channel 0, MR0
  TIM_MatchConfigStruct.MatchChannel = 2;
  // Disable interrupt when MR0 matches the value in TC register
  TIM_MatchConfigStruct.IntOnMatch   = TRUE;
  //Enable reset on MR0: TIMER will reset if MR0 matches it
  TIM_MatchConfigStruct.ResetOnMatch = TRUE;
  //Stop on MR0 if MR0 matches it
  TIM_MatchConfigStruct.StopOnMatch  = TRUE;
  //Toggle MR0.0 pin if MR0 matches it
  TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
  // Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10Hz)
  TIM_MatchConfigStruct.MatchValue   = value;
  
  // Set configuration for Tim_config and Tim_MatchConfig
  TIM_Init(LPC_TIM2, TIM_TIMER_MODE,&TIM_ConfigStruct);
  TIM_ConfigMatch(LPC_TIM2,&TIM_MatchConfigStruct);
  TIM_Cmd(LPC_TIM2,ENABLE);
  while ( !(TIM_GetIntStatus(LPC_TIM2,TIM_MR2_INT)));
  TIM_ClearIntPending(LPC_TIM2,TIM_MR2_INT);
  TIM_Cmd(LPC_TIM2,DISABLE);
}

void delay_ms_interrupt(uint32_t value)
{
  //timer init
  TIM_TIMERCFG_Type TIM_ConfigStruct;
  TIM_MATCHCFG_Type TIM_MatchConfigStruct;
  value = value * 1000;
  
  TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_ConfigStruct.PrescaleValue	= 1;
  
  // use channel 0, MR0
  TIM_MatchConfigStruct.MatchChannel = 0;
  // Disable interrupt when MR0 matches the value in TC register
  TIM_MatchConfigStruct.IntOnMatch   = TRUE;
  //Enable reset on MR0: TIMER will reset if MR0 matches it
  TIM_MatchConfigStruct.ResetOnMatch = TRUE;
  //Stop on MR0 if MR0 matches it
  TIM_MatchConfigStruct.StopOnMatch  = TRUE;
  //Toggle MR0.0 pin if MR0 matches it
  TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
  // Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10Hz)
  TIM_MatchConfigStruct.MatchValue   = value;
  
  // Set configuration for Tim_config and Tim_MatchConfig
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
  TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);
  
  
  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(BRD_TIM_INTR_USED, ((0x01<<3)|0x01));
  
  /* Enable interrupt for timer 0 */
  NVIC_EnableIRQ(BRD_TIM_INTR_USED);
  
  // To start timer
  TIM_Cmd(BRD_TIMER_USED, ENABLE);
}

void Disable_Timer0(void)
{
  TIM_Cmd(BRD_TIMER_USED, DISABLE);
  //NVIC_DisableIRQ(BRD_TIM_INTR_USED);
}


/*********************************************************************//**
 * @brief		TIMER2 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER3_IRQHandler(void)
{
  if (TIM_GetIntStatus(LPC_TIM3, TIM_MR3_INT)== SET)
  {
    //int r = 0;
    //_DBG_("Match interrupt occur...");
    //m_iFlagRecieveFrame = 1;
    //m/_iFlagRecieveUARTFrame = 1;
		//SendStatusToDevice();
  }
  
  
  TIM_ClearIntPending(LPC_TIM3, TIM_MR3_INT);
}


void Initialize_Timer3(uint32_t value)
{
	
	TIM_TIMERCFG_Type TIM_ConfigStruct;
  TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	
  value = value * 1000;
	
	if(value == 0)
		return;
	
	  TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
  TIM_ConfigStruct.PrescaleValue	= 1;
  
  // use channel 0, MR0
  TIM_MatchConfigStruct.MatchChannel = 3;
  // Disable interrupt when MR0 matches the value in TC register
  TIM_MatchConfigStruct.IntOnMatch   = TRUE;
  //Enable reset on MR0: TIMER will reset if MR0 matches it
  TIM_MatchConfigStruct.ResetOnMatch = TRUE;
  //Stop on MR0 if MR0 matches it
  TIM_MatchConfigStruct.StopOnMatch  = FALSE;
  //Toggle MR0.0 pin if MR0 matches it
  TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
  // Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10Hz)
  TIM_MatchConfigStruct.MatchValue   = value;
  
  // Set configuration for Tim_config and Tim_MatchConfig
  TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
  TIM_ConfigMatch(LPC_TIM3,&TIM_MatchConfigStruct);
  TIM_Cmd(LPC_TIM3,ENABLE);
	
	

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER3_IRQn, ((0x01<<3)|0x01));

	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER3_IRQn);

	// To start timer 0
	TIM_Cmd(LPC_TIM3, ENABLE);
}

