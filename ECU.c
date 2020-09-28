/* WARNING: Auto generated file. Do not modify. */
/*  Siemens Petrol Euro 2 */

#include "ECU.h"
#include "ECU_define.h"
#include "ECU_helper.h"


extern volatile STRING_BUFFER scheduler_output_buffer[NUMBER_OF_BUFFER_FOR_OUTPUT];
extern volatile SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
extern CMD_STRUCTURE m_CmdStruct;
extern unsigned char ecuStatusProgrammed[5];
extern volatile uint8_t m_ConnectionStatus;
void check_7f(uint8_t * buf)
{
 		if(m_frmBank[0].frm[3] == 0x7f)
		{
				sprintf(buf, "NaN");
		}
}

/* frames 3e */
void task_standby(char * input,int num) {
	GeneralStandbyProcedure();
}

/* frames 10,80 */
void task_connect(char * input,int num) {
	GeneralConnectProcedure();
}

void task_disconnect(char * input,int num) {
	GeneralDisconnectProcedure(1);
}

/* frames 18,0,0,0 */
void task_summary_18000(char * input,int num) {
	uint8_t index_buffer_i = 0;
	uint8_t * buf = 0;
	uint8_t enable_or_disable = 0;
	uint8_t exceptional_value = 0;

	/* Number of Faults */
	if(checkIdInArray(PARAM_320838))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		if(m_frmBank[0].frm[3] == 0x7f)
		{
				ecuStatusProgrammed[0] = 3;
		}
		else
		{
			ecuStatusProgrammed[0] = 0x60;
			sprintf(buf, "%d", m_frmBank[0].frm[4]);
		}
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_320838;
		check_7f(buf); index_buffer_i++;
	}


}


/* frames 14,0,0 */
void task_faultClean(char * input,int num) {
char res = 0;
  char return_value;
	char ret_frame[3];
  //res = RunTheSpecificTask(&task[STANDBY]);
	
	taskDiagRAM tskHelper;
  taskDiagRAM * tskPointer = &tskHelper;
	
  ConvertTask(task[FAULT_ERASE], tskPointer);
  
  res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
                                                                tskPointer->frmFormat[0].frm, 
                                                                tskPointer->frmFormat[0].frm_size,0);


	//if error                                                              
   if(m_frmBank[0].frm[3] == 0x7f)
  {
    //report the errors!!!
    if(m_frmBank[0].frm[5] == 0x22)
    {
      //emkan pak kardan khata nist, motor roshan ast
      ret_frame[0] = 1;
    }
    else
    {
      //emkan pak kardan khata nist, sayer
      ret_frame[0] = 2;
    }
  }
  else //OK
  {
     ret_frame[0] = 0;
  }
	
	
  return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
	
	//SetTheECUToSpecificState(STANDBY);
	m_Scheduler_Buffer.CMD = CMD2_TASK_DATA_FAULTS;
	
	
	//no send the results :
  SendGeneralFrame(ret_frame, 1, CMD1_TASK_DATA, CMD2_TASK_DATA_ERASEFAULT, 0, 0);
	delay_ms(500);
}

/* frames 21,01 */
//#pragma push
//#pragma O1
void task_parameter_2101(char * input,int num) {
	uint8_t index_buffer_i = 0;
	uint8_t * buf = 0;
	uint8_t enable_or_disable = 0;
	uint8_t exceptional_value = 0;

	/* Battery Voltage */
	if(checkIdInArray(PARAM_34711))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%2.1f", (m_frmBank[0].frm[6] * 0.1015953));
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34711;
		check_7f(buf); index_buffer_i++;
	}

	/* Throttle Position Voltage */
	if(checkIdInArray(PARAM_34717))
	{
		float throttle_volt;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    throttle_volt = m_frmBank[0].frm[12]*0.0195312;
		    sprintf(buf, "%2.2f", throttle_volt);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34717;
		check_7f(buf); index_buffer_i++;
	}

	/* Coolant Temperature */
	if(checkIdInArray(PARAM_34721))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%.0f", m_frmBank[0].frm[9]*0.75 - 48); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34721;
		check_7f(buf); index_buffer_i++;
	}

	/* Coolant temperature voltage */
	if(checkIdInArray(PARAM_34725))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%.2f", m_frmBank[0].frm[8]*0.0195312);   
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34725;
		check_7f(buf); index_buffer_i++;
	}

	/* Manifold Air Temperature */
	if(checkIdInArray(PARAM_34729))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		    sprintf(buf, "%.0f", m_frmBank[0].frm[11]*0.747 -48);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34729;
		check_7f(buf); index_buffer_i++;
	}

	/* Manifold Air Temperature Voltage Sensor */
	if(checkIdInArray(PARAM_34732))
	{
		float manifold_temp;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    manifold_temp = m_frmBank[0].frm[10]*0.0195312;
		    sprintf(buf, "%2.2f", manifold_temp); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34732;
		check_7f(buf); index_buffer_i++;
	}

	/* Manifold Pressure */
	if(checkIdInArray(PARAM_34735))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		sprintf(buf, "%.0f", makeint(m_frmBank[0].frm[20], m_frmBank[0].frm[19]) * 0.0829175);    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34735;
		check_7f(buf); index_buffer_i++;
	}

	/* Manifold Pressure Voltage Sensor */
	if(checkIdInArray(PARAM_34738))
	{
		float manifold_press;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    manifold_press = m_frmBank[0].frm[16]*0.0195312;
		    sprintf(buf, "%2.2f", manifold_press);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34738;
		check_7f(buf); index_buffer_i++;
	}

	/* vehicle speed */
	if(checkIdInArray(PARAM_34741))
	{
		float vehicle_speed;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    vehicle_speed = m_frmBank[0].frm[24];
		    sprintf(buf, "%.0f", vehicle_speed); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34741;
		check_7f(buf); index_buffer_i++;
	}

	/* Coil charging Timing */
	if(checkIdInArray(PARAM_34745))
	{
		float Charging_time;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    Charging_time = (m_frmBank[0].frm[79] * 256 + m_frmBank[0].frm[78]) * 0.004;//f11
		    sprintf(buf, "%2.1f", Charging_time);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34745;
		check_7f(buf); index_buffer_i++;
	}

	/* Idle Adaptation */
	if(checkIdInArray(PARAM_34772))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		sprintf(buf, "%.0f", makesignint(m_frmBank[0].frm[67], m_frmBank[0].frm[66]) * 0.00152592); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34772;
		check_7f(buf); index_buffer_i++;
	}

	/* Stepper Motor */
	if(checkIdInArray(PARAM_34775))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		    sprintf(buf, "%d", m_frmBank[0].frm[68]); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34775;
		check_7f(buf); index_buffer_i++;
	}

	/* Ignition Advance */
	if(checkIdInArray(PARAM_34778))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		    sprintf(buf, "%.0f", 72 - m_frmBank[0].frm[35] * 0.375); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34778;
		check_7f(buf); index_buffer_i++;
	}

	/* Upstream O2 Sensor Voltage */
	if(checkIdInArray(PARAM_34781))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%2.2f", ((m_frmBank[0].frm[32] & 0x03) * 256 + m_frmBank[0].frm[31]) * 0.0048828);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34781;
		check_7f(buf); index_buffer_i++;
	}

	/* Upstream O2 Sensor Heating */
	if(checkIdInArray(PARAM_34785))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%.0f", m_frmBank[0].frm[60] * 0.392156); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34785;
		check_7f(buf); index_buffer_i++;
	}

	/* Idle */
	if(checkIdInArray(PARAM_34789))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		 
		sprintf(buf, "%d", makeint(m_frmBank[0].frm[28], m_frmBank[0].frm[27])); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34789;
		check_7f(buf); index_buffer_i++;
	}

	/* Canister Valve Openning */
	if(checkIdInArray(PARAM_34792))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		sprintf(buf, "%.0f", ((m_frmBank[0].frm[72] & 0x7f) * 256 + m_frmBank[0].frm[71]) * 0.003052);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34792;
		check_7f(buf); index_buffer_i++;
	}

	/* Mean O2 Correct */
	if(checkIdInArray(PARAM_34795))
	{
		float mean_o2;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		
		if((m_frmBank[0].frm[55]) > 0x7F)
					mean_o2 = -(0xFFFF - (m_frmBank[0].frm[55]* 256 +  m_frmBank[0].frm[54])) * 0.00152592;
			  else
				  mean_o2 = (m_frmBank[0].frm[55]* 256 + m_frmBank[0].frm[54]) * 0.00152592;
						
				sprintf(buf, "%.0f", mean_o2); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34795;
		check_7f(buf); index_buffer_i++;
	}

	/* Mileage */
	if(checkIdInArray(PARAM_34798))
	{
		int mileage = 0;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		mileage = (m_frmBank[0].frm[84] * 16777216 + m_frmBank[0].frm[83] * 65536 + m_frmBank[0].frm[82] * 256 + m_frmBank[0].frm[81]) * 0.1;
		    sprintf(buf, "%d", mileage);    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34798;
		check_7f(buf); index_buffer_i++;
	}

	/* Engine State */
	if(checkIdInArray(PARAM_34801))
	{
		int engine_state = 0;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    switch(m_frmBank[0].frm[77])
				{
					case 1 :
						engine_state = 0;
						break;
					case 2 :
						engine_state = 1;
						break;
					case 4 :
						engine_state = 2;
						break;
					case 8 :
						engine_state = 3;
						break;
					case 16 :
						engine_state = 4;
						break;
					case 32 :
						engine_state = 5;
						break;
					default :
						engine_state = 6;
						break;
					
					
			  }
				sprintf(buf, "%d", engine_state);
				
				
				
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34801;
		check_7f(buf); index_buffer_i++;
	}

	/* Fan Relay */
	if(checkIdInArray(PARAM_34811))
	{
		float fan_relay = 0;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    fan_relay = m_frmBank[0].frm[75] & 0x03;
		    sprintf(buf, "%.0f", fan_relay);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34811;
		check_7f(buf); index_buffer_i++;
	}

	/* Fuel pump status */
	if(checkIdInArray(PARAM_34880))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[7],7));  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34880;
		check_7f(buf); index_buffer_i++;
	}

	/* Fuel Cut */
	if(checkIdInArray(PARAM_34904))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[77],4));  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34904;
		check_7f(buf); index_buffer_i++;
	}

	/* Richness Regulation Status */
	if(checkIdInArray(PARAM_34912))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[51],0));  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34912;
		check_7f(buf); index_buffer_i++;
	}

	/* Gear */
	if(checkIdInArray(PARAM_34928))
	{
		float gear;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    gear = m_frmBank[0].frm[85];
		    if (gear > 5) 
					gear = 6; 
		    sprintf(buf, "%.0f",gear);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34928;
		check_7f(buf); index_buffer_i++;
	}

	/* Run Time With VSS Error */
	if(checkIdInArray(PARAM_34940))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		 
		sprintf(buf, "%.1f", makeint(m_frmBank[0].frm[91],m_frmBank[0].frm[90])*1.8204444 + makeint(m_frmBank[0].frm[89],m_frmBank[0].frm[88]) * 0.0000278);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34940;
		check_7f(buf); index_buffer_i++;
	}

	/* Vehicle Speed Sensor Error */
	if(checkIdInArray(PARAM_34943))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    sprintf(buf, "%d", m_frmBank[0].frm[87]);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34943;
		check_7f(buf); index_buffer_i++;
	}

	/* Additive Correct Mixture Adaptive */
	if(checkIdInArray(PARAM_34946))
	{
		
		float AIT;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		
		if((m_frmBank[0].frm[57]) > 0x7F)
					AIT = -(0xFFFF - (m_frmBank[0].frm[57] * 256 + m_frmBank[0].frm[56])) * 0.004;
			  else
				  AIT = (m_frmBank[0].frm[57] * 256 + m_frmBank[0].frm[56]) * 0.004;
						
		sprintf(buf, "%.0f", AIT);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34946;
		check_7f(buf); index_buffer_i++;
	}

	/* Multiple Correct Mixture Adaptive */
	if(checkIdInArray(PARAM_34949))
	{
		float MIT;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		  
		if((m_frmBank[0].frm[59]) > 0x7F)
					MIT = -(0xFFFF - (m_frmBank[0].frm[59]* 256 +  m_frmBank[0].frm[58])) * 0.00152587;
			  else
				  MIT = (m_frmBank[0].frm[59]* 256 + m_frmBank[0].frm[58]) * 0.00152587;
						
				sprintf(buf, "%.0f", MIT);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34949;
		check_7f(buf); index_buffer_i++;
	}

	/* Coolong Fan Voltage */
	if(checkIdInArray(PARAM_34952))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%.2f", m_frmBank[0].frm[86] * 0.01953137);    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34952;
		check_7f(buf); index_buffer_i++;
	}

	/* Air Condition Switch */
	if(checkIdInArray(PARAM_34955))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[30],0));    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34955;
		check_7f(buf); index_buffer_i++;
	}

	/* Idle correction A/C on */
	if(checkIdInArray(PARAM_34964))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[30],4));  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_34964;
		check_7f(buf); index_buffer_i++;
	}

	/* A/C Compressor Relay */
	if(checkIdInArray(PARAM_35040))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[30],2));    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35040;
		check_7f(buf); index_buffer_i++;
	}

	/* MAF Correct Altitude */
	if(checkIdInArray(PARAM_35045))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		sprintf(buf, "%d", m_frmBank[0].frm[23]);   
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35045;
		check_7f(buf); index_buffer_i++;
	}

	/* Switch Status */
	if(checkIdInArray(PARAM_35055))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[7],0));   
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35055;
		check_7f(buf); index_buffer_i++;
	}

	/* Knock */
	if(checkIdInArray(PARAM_35071))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[80],0)); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35071;
		check_7f(buf); index_buffer_i++;
	}

	/* Throttle Position Angle */
	if(checkIdInArray(PARAM_35128))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%2.1f", m_frmBank[0].frm[13]*0.46862745);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35128;
		check_7f(buf); index_buffer_i++;
	}

	/* Mass Air Flow */
	if(checkIdInArray(PARAM_35145))
	{
		float mass_air_flow;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    mass_air_flow = makeint(m_frmBank[0].frm[18], m_frmBank[0].frm[17]) * 0.03125;
		    sprintf(buf, "%.0f", mass_air_flow); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35145;
		check_7f(buf); index_buffer_i++;
	}

	/* Engine Speed */
	if(checkIdInArray(PARAM_35191))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		if(m_frmBank[0].frm[3] == 0x7f)
		{
			if(ecuStatusProgrammed[0] != 0x60)
				ecuStatusProgrammed[1] = 0x00;
			else
				ecuStatusProgrammed[1] = 0x60;
		}
		else
			ecuStatusProgrammed[1] = 0x60; 
		   
		sprintf(buf, "%d", makeint(m_frmBank[0].frm[26], m_frmBank[0].frm[25]));   
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35191;
		check_7f(buf); index_buffer_i++;
	}

	/* Evaporator temp */
	if(checkIdInArray(PARAM_35194))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		    sprintf(buf, "%.0f", m_frmBank[0].frm[29] * 0.0980392); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35194;
		check_7f(buf); index_buffer_i++;
	}

	/* AC Pressure switch */
	if(checkIdInArray(PARAM_35207))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[30],3)); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35207;
		check_7f(buf); index_buffer_i++;
	}

	/* Air Condition Thermostat  Switch */
	if(checkIdInArray(PARAM_35213))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    
		    sprintf(buf, "%d", checkbiter(m_frmBank[0].frm[30],1));  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35213;
		check_7f(buf); index_buffer_i++;
	}

	/* Injection Time */
	if(checkIdInArray(PARAM_35218))
	{
		float inject_time;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    inject_time = makeint(m_frmBank[0].frm[48], m_frmBank[0].frm[47]) * 0.004;
		    sprintf(buf, "%2.1f", inject_time);  
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35218;
		check_7f(buf); index_buffer_i++;
	}

	/* Start Angle of Injection */
	if(checkIdInArray(PARAM_35235))
	{
		float SOI = 0;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		     
		SOI =  180 - (m_frmBank[0].frm[49] * 4.6875);
		    sprintf(buf, "%2.1f", SOI);    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35235;
		check_7f(buf); index_buffer_i++;
	}

	/* Fuel shot off pattern */
	if(checkIdInArray(PARAM_35239))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    sprintf(buf, "%d", m_frmBank[0].frm[50]);    
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35239;
		check_7f(buf); index_buffer_i++;
	}

	/* Idle speed actuator opening */
	if(checkIdInArray(PARAM_35244))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	

		    sprintf(buf, "%2.1f", makeint(m_frmBank[0].frm[63], m_frmBank[0].frm[62]) * 0.001525902189);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35244;
		check_7f(buf); index_buffer_i++;
	}

	/* Idle speed regulation */
	if(checkIdInArray(PARAM_35249))
	{
		float ISR;
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		   
		if((m_frmBank[0].frm[65]) > 0x7F)
				ISR = -(0xFFFF - (m_frmBank[0].frm[65]* 256 +  m_frmBank[0].frm[64])) * 0.00152592;
	  else
				ISR = (m_frmBank[0].frm[65]* 256 + m_frmBank[0].frm[64]) * 0.00152592;
						
	  sprintf(buf, "%.0f", ISR); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35249;
		check_7f(buf); index_buffer_i++;
	}

	/* Canister loading */
	if(checkIdInArray(PARAM_35297))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
	
		    sprintf(buf, "%2.1f", makeint(m_frmBank[0].frm[74], m_frmBank[0].frm[73]) * 1.220703125047684443427176317998e-4);
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35297;
		check_7f(buf); index_buffer_i++;
	}

	/* Target of stepper motor position */
	if(checkIdInArray(PARAM_35301))
	{
		enable_or_disable = 0;
		exceptional_value = 0;
		buf = scheduler_output_buffer[index_buffer_i].buf[index_buffer_j];
	
		    sprintf(buf, "%d", m_frmBank[0].frm[76]); 
	
		scheduler_output_buffer[index_buffer_i].enable_or_disable = enable_or_disable;
		scheduler_output_buffer[index_buffer_i].exceptional_value = exceptional_value;
		scheduler_output_buffer[index_buffer_i].item_num = PARAM_35301;
		check_7f(buf); index_buffer_i++;
	}


}

//#pragma pop
/* frames 1a,8d */
void task_identification_1a8d(char * input,int num) {
  char i = 0;
  char * buf = 0;
  char enable_or_disable = 0;char exceptional_value = 0;
  
  /*Software Version*/
  if(checkIdInArray(PARAM_36639))
  {
    enable_or_disable = 0;exceptional_value = 0;
    buf = scheduler_output_buffer[i].buf[index_buffer_j];
    
		array_copy(buf,0,11,m_frmBank[0].frm,5,16);
		buf[12] = 0;
		
    scheduler_output_buffer[i].enable_or_disable = enable_or_disable;scheduler_output_buffer[i].exceptional_value =exceptional_value;
		scheduler_output_buffer[i].item_num =  PARAM_36639;
    i++;
  }
  
  
}

/* frames 1a,8e */
void task_identification_1a8e(char * input,int num) {
  char i = 0;
  char * buf = 0;
  char enable_or_disable = 0;char exceptional_value = 0;
  
  /*Calibration Number*/
  if(checkIdInArray(PARAM_36653))
  {
    enable_or_disable = 0;exceptional_value = 0;
    buf = scheduler_output_buffer[i].buf[index_buffer_j];
		
		array_copy(buf,0,11,m_frmBank[0].frm,5,16);
		buf[12] = 0;
		
    
    scheduler_output_buffer[i].enable_or_disable = enable_or_disable;scheduler_output_buffer[i].exceptional_value =exceptional_value;
		scheduler_output_buffer[i].item_num =  PARAM_36653;
    i++;
  }
  
  
}


extern int number_of_task;
int m_counter_fault;
uint8_t old_buf[512] = {0};

/* frames 18,0,0,0 */
void task_faults_18000(char * input,int num) {
	
	int i = 0;
	int index = 0;
	int index_1 = 0;
	uint8_t * te;
	//int number_of_fault = (m_frmBank[0].frm[0] - 0x7c - 6)/3;
	int number_of_fault = m_frmBank[0].frm[4];
	//FAULT_RESOLVER * fault_resol = (FAULT_RESOLVER *) malloc(sizeof(FAULT_RESOLVER) * number_of_fault);
	
	uint8_t * buf;
  if(m_frmBank[0].frm[3] != 0x7F)
  {
		buf = (uint8_t *) malloc(6 * number_of_fault + 1);
	
		buf[index_1++] = number_of_fault;

	for (i = 0 ; i < number_of_fault ; i++)
	{
		//
		index = i * 3 + 5;
		buf[index_1++] = m_frmBank[0].frm[index+1];
		buf[index_1++] = m_frmBank[0].frm[index];
		buf[index_1++] = 0;
		buf[index_1++] = 0;
		buf[index_1++] = m_frmBank[0].frm[index+2] & 0x1f;
		
		//the status of fault:
	
		
		if(checkbiter(m_frmBank[0].frm[index+2],6))
		{
			buf[index_1] = 1;
		}
		else
		{
			buf[index_1] = 2;
		}
		
		index_1++;
	}
	
	
	ecuStatusProgrammed[0] = 0x60;
 }
	else
	{
		ecuStatusProgrammed[0] = 0x03;
		index_1 = 0;
		buf = (uint8_t *) malloc(1);
		number_of_fault = 0;
		
	}
	
   SendFaultFrame(buf,index_1,number_of_fault);


	free(buf);
}


char AnalyzeTheActuatorResults(char * res)
{
  char condition = 0;
  
  if(m_frmBank[0].frm[3] == 0x7f)
  {
    //report the errors!!!
    if(m_frmBank[0].frm[5] == 0x22)
    {
      condition = 2;
    }
		else
    {
      condition = 3;
    }
  }
  else
  {
    condition = 0;
  }
  
  return condition;
}


/* frames 30,00,07,ff | 30,3f,00 */
void task_actuator_300007ff(char * input,int num) {
	
	int res = 0;
	char condition = 0;
	int act_num = 0;
	int act_type =0;
	char ret_frame[] = {0x11};
   const taskDiag * tskPointer = m_Scheduler_Buffer.tskDiag;
	const char * array_data_chr_t37;
	TYPE_DATA_INTERNAL * data_internal;
  //taskDiag * tskPointer= NULL;
   uint8_t Actfrm[10] =  {0};
	 int ActfrmNum;
  memcpy_P(tskPointer->frmFormat[0].frm, Actfrm, 10);
  ActfrmNum = tskPointer->frmFormat[0].frm_size;
  
  //start the task!!!!
  //Actfrm[1] = input[0];
	 data_internal = (TYPE_DATA_INTERNAL *)tskPointer->point_internal_data;
	 array_data_chr_t37 = data_internal->point_char_data;
	 //array_data_chr_t37 = tskPointer->point_internal_data.point_char_data;
	 
	Actfrm[1] = array_data_chr_t37[checkSpecificDataInArrayFromFlash(makeint(input[1],input[0]), 
	 tskPointer->index_array, 
	 data_internal->size_of_data)];
	Actfrm[3] = 0xff;
	 
	 act_num = Actfrm[1];
	 
	 if((act_num == 0x01) || (act_num == 0x02) || (act_num == 0x04) || (act_num == 0x08))
	 {
		 act_type = 1;
	 }
	 
  
	if(act_type == 1)
	{
		Actfrm[1] = 0x30;
		Actfrm[2] = 0x07;
		Actfrm[3] = act_num;
	}
	
  res = profile_E_20063.protocol_solver->SendAndRecieveFrame(profile_E_20063.spect,
                                                       Actfrm, 
                                                       ActfrmNum,
                                                       0);
	profile_E_20063.protocol_solver->CheckTheResultsAndPerformAction(res);
  
  condition = AnalyzeTheActuatorResults(m_frmBank[0].frm);
	
	delay_ms(150);
  
  if(condition == 0x00 && res == STATUS_ECU_CONNECTION_OKAY)
  {

      int repeat = 0;
      while(repeat != 30)
      {
        if(m_CmdStruct.m_cmd_1 == 3)  //canceled by user
        {
          condition = 1;
          break;
        }
        
				
        GeneralStandbyProcedure();
				
				delay_ms(250);
       
        repeat++;
      }
      
      //send the end frame : 
			if(act_type == 0)
			{
      Actfrm[1] = act_num;Actfrm[3] = 0x00;
      res = profile_E_20063.protocol_solver->SendAndRecieveFrame(profile_E_20063.spect,
                                                           Actfrm, 
                                                           ActfrmNum,
                                                           0); 
			profile_E_20063.protocol_solver->CheckTheResultsAndPerformAction(res);
  
      condition = AnalyzeTheActuatorResults(m_frmBank[0].frm);
				
			delay_ms(150);
			}
			
			//83 11 F1 30 3F 00 F4 83 F1 11 70 3F 00 34
			
			Actfrm[1] = 0x3f;Actfrm[2] = 0x00;
     res = profile_E_20063.protocol_solver->SendAndRecieveFrame(profile_E_20063.spect,
                                                           Actfrm, 
                                                           3,
                                                           0); 
			profile_E_20063.protocol_solver->CheckTheResultsAndPerformAction(res);
  
      condition = AnalyzeTheActuatorResults(m_frmBank[0].frm);
			
			delay_ms(150);
			
			
  }
	else if(res != STATUS_ECU_CONNECTION_OKAY)
	{
		condition = 3;
	}
  
	ret_frame[0] = condition;
  SendGeneralFrame(ret_frame, 1, CMD1_TASK_DATA, CMD2_TASK_DATA_ACTUATOR, 0, 0);
  m_Scheduler_Buffer.send_to_debug = 0;
		if(res == STATUS_ECU_CONNECTION_OKAY)
			SetTheECUToSpecificState(STANDBY);
	m_CmdStruct.CMD_State = 0;
	m_CmdStruct.m_cmd_1 = 0;
	m_CmdStruct.m_cmd_2 = 0;
	m_CmdStruct.m_cmd_3 = 0;
	
	
	
}

/* frames 30,00,04 */
void task_actuator_300004(char * input,int num) {

	int res = 0;
	char condition = 0;
	int act_num = 0;
	int act_type =0;
	char ret_frame[] = {0x11};
  const taskDiag * tskPointer = m_Scheduler_Buffer.tskDiag;
	const char * array_data_chr_t37;
	TYPE_DATA_INTERNAL * data_internal;
  //taskDiag * tskPointer= NULL;
   uint8_t Actfrm[10] =  {0};
	 int ActfrmNum;
  memcpy_P(tskPointer->frmFormat[0].frm, Actfrm, 10);
  ActfrmNum = tskPointer->frmFormat[0].frm_size;
  
  //start the task!!!!
  //Actfrm[1] = input[0];
	 data_internal = (TYPE_DATA_INTERNAL *)tskPointer->point_internal_data;
	 array_data_chr_t37 = data_internal->point_char_data;
	 //array_data_chr_t37 = tskPointer->point_internal_data.point_char_data;
	 
	Actfrm[1] = array_data_chr_t37[checkSpecificDataInArrayFromFlash(makeint(input[1],input[0]), 
	 tskPointer->index_array, 
	 data_internal->size_of_data)];
	//Actfrm[3] = 0xff;
	 
	 act_num = Actfrm[1];
	 
	  Actfrm[0] = 0x30;
		Actfrm[1] = act_num;
		Actfrm[2] = 0x04;
	
	
  res = profile_E_20063.protocol_solver->SendAndRecieveFrame(profile_E_20063.spect,
                                                       Actfrm, 
                                                       ActfrmNum,
                                                       0);
  
  profile_E_20063.protocol_solver->CheckTheResultsAndPerformAction(res);
					
	condition = AnalyzeTheActuatorResults(m_frmBank[0].frm);
	
	delay_ms(150);
  
	
	if(res != STATUS_ECU_CONNECTION_OKAY)
	{
		condition = 3;
	}
	else
		condition = 0;
  
	ret_frame[0] = condition;
	SendGeneralFrame(ret_frame, 1, CMD1_TASK_DATA, CMD2_TASK_DATA_ACTUATOR, 0, 0);

  m_Scheduler_Buffer.send_to_debug = 0;
		if(res == STATUS_ECU_CONNECTION_OKAY)
			SetTheECUToSpecificState(STANDBY);
	m_CmdStruct.CMD_State = 0;
	m_CmdStruct.m_cmd_1 = 0;
	m_CmdStruct.m_cmd_2 = 0;
	m_CmdStruct.m_cmd_3 = 0;
	
}
/* WARNING: Auto generated file. Do not modify. */
