#include "ECU_helper.h"
#include "CommProtocol.h"
#include "ECU_define.h"
#include "relayController.h"

extern	const taskDiag task[];
extern int number_of_task;
extern ProtocolProfiler * m_CurrentProf;
extern STRING_BUFFER scheduler_output_buffer[NUMBER_OF_BUFFER_FOR_OUTPUT];
extern STRING_BUFFER scheduler_output_buffer_final[NUMBER_OF_BUFFER_FOR_OUTPUT];
extern STRING_BUFFER scheduler_output_buffer_comparator[NUMBER_OF_BUFFER_FOR_OUTPUT];
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;
extern uint8_t m_jump_Condition;
 uint8_t ConnectionStatus;
CMD_STRUCTURE m_CmdStruct;
extern volatile uint8_t m_ConnectionStatus;
extern unsigned char xmegatx_frame[256];
extern unsigned char xmegarx_frame[256];
extern volatile uint8_t m_FirstPromptDC;
extern unsigned char ecuStatusProgrammed[5];

char cmd_mapping[] = {CMD2_TASK_DATA_STANDBY, 
CMD2_TASK_DATA_CONNECT, 
CMD2_TASK_DATA_DISCONNECT,
CMD2_TASK_DATA_PARAMETER,
CMD2_TASK_DATA_CONFIGURATION, 
CMD2_TASK_DATA_REMOTE_LEARNING,
CMD2_TASK_DATA_FAULTS, 
CMD2_TASK_DATA_DOWNLOAD, 
CMD2_TASK_DATA_CATEGORY, 
CMD2_TASK_DATA_ZIR_ACTUATOR, 
CMD2_TASK_DATA_ACTUATOR, 
CMD2_TASK_DATA_SUMMARY,
CMD2_TASK_DATA_IDENTIFICATION};

void GeneralStandbyWithOutDelay(void)
{
	char return_value;
	int res = 0;
	//res = RunTheSpecificTask(&task[STANDBY]);
	
	taskDiagRAM tskHelper;
	taskDiagRAM * tskPointer = &tskHelper;
	
	ConvertTask(task[STANDBY], tskPointer);
	
	res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																tskPointer->frmFormat[0].frm, 
																																tskPointer->frmFormat[0].frm_size,0);

	return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
}


void GeneralStandbyProcedure(void)
{
		
	char res = 0;
	char return_value;
	//res = RunTheSpecificTask(&task[STANDBY]);
	
	taskDiagRAM tskHelper;
	taskDiagRAM * tskPointer = &tskHelper;
	
	ConvertTask(task[STANDBY], tskPointer);
	
	res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																tskPointer->frmFormat[0].frm, 
																																tskPointer->frmFormat[0].frm_size,0);

	return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
	
	if(res == STATUS_ECU_CONNECTION_OKAY)
		delay_ms_standby(task[STANDBY].frmFormat[0].delay_between_frame);
}


uint8_t check_two_buffer(uint8_t * dest, uint8_t * src, uint32_t size_e)
{
	int i = 0;
	for (i = 0; i < size_e ; i++)
	{
		if(dest[i] != src[i])
		{
			return 1;
		}
	}
	
	return 0;
}

void GeneralDisconnectProcedure(char change_status)
{
		char res = 0;
	char return_value;
	//res = RunTheSpecificTask(&task[STANDBY]);
	
	taskDiagRAM tskHelper;
	taskDiagRAM * tskPointer = &tskHelper;
	
	ConvertTask(task[DISCONNECT], tskPointer);
	
	res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																tskPointer->frmFormat[0].frm, 
																																tskPointer->frmFormat[0].frm_size,0);
	
	if(change_status == 1)
		m_ConnectionStatus = CONNECTION_TO_CAR_NO;
	//delay_ms(1000);
	//SetTheECUToSpecificState(0xff);

}

uint8_t GeneralInitConnectProcedure(char counter_limit)
{
		char condition = 0;
	char res = 0xff;
	char try_num = 0;
	char status_con = 0;
	
	char counter = 0;
	
	taskDiagRAM tskDg;
	taskDiagRAM * tskPointer = &tskDg;
	
	ConvertTask(task[CONNECT], tskPointer);
	
	//try three times :::
	
	while(counter < counter_limit)
	{
		
		if(m_CurrentProf->spect->Kwp_spect != NULL)
		{
						//connect the short :
	if(checkbiter(m_CurrentProf->spect->Kwp_spect->init_variatity, 0))
	{
		//short connection :
		m_CurrentProf->spect->Kwp_spect->init_type = INIT_SHORT_TYPE;
		
		m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect);
		res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																	tskPointer->frmFormat[0].frm, 
																																	tskPointer->frmFormat[0].frm_size,0);

		//now apply the delay :
		delay_ms(m_CurrentProf->spect->Kwp_spect->short_init->delay_after_init);
		
	}
	
	
	if(res != STATUS_ECU_CONNECTION_OKAY)
	{
		if(checkbiter(m_CurrentProf->spect->Kwp_spect->init_variatity, 1))
		{
			m_CurrentProf->spect->Kwp_spect->init_type = INIT_LONG_TYPE;
			res = m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect);
			
			delay_ms(m_CurrentProf->spect->Kwp_spect->long_init->delay_after_init);
		}
	}
		}
		else
		{
					m_CurrentProf->protocol_solver->ConnectProcedure(m_CurrentProf->spect);
		res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																	tskPointer->frmFormat[0].frm, 
																																	tskPointer->frmFormat[0].frm_size,0);
		}

	
	if(res == STATUS_ECU_CONNECTION_OKAY)
	{
		m_FirstPromptDC = 0;
		Activate_LEDPin(LED_CONNECT,ON_MODE);
		m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
		break;
	}
	else
	{
		counter++;
	}

	}

	return res;

}


void GeneralConnectProcedure(void)
{
	
	char condition = 0;
	char res = 0;
	char try_num = 0;
	char status_con = 0;
	
	char counter = 0;
	
	for (counter = 0 ; counter < 5; counter++)
		ecuStatusProgrammed[counter] = 0x60;
	
	if(m_ConnectionStatus == CONNECTION_TO_CAR_OKAY)
	{
		res = STATUS_ECU_CONNECTION_OKAY;
	}
	else
	{
		res = GeneralInitConnectProcedure(3);
	}
	
	
	//res = GeneralInitConnectProcedure();


	if(res == STATUS_ECU_CONNECTION_OKAY)
	{
		char ret_frame[] = { 0x11, 1};
		ConnectionStatus = 1;
		//SetTheECUToSpecificState(STANDBY);
		SetTheECUToSpecificState(0xff);

		
		delay_ms(100);
		GeneralStandbyWithOutDelay();
		delay_ms(25);
		
		
		
		m_ConnectionStatus = CONNECTION_TO_CAR_OKAY;
		m_FirstPromptDC = 0;
		Activate_LEDPin(LED_CONNECT,ON_MODE);
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_CONNECT , ret_frame , sizeof(ret_frame));

	 
		//return SCHEDULER_STATUS_GO_TO_DEFAULT;
	}
	else
	{
		char ret_frame[] = {0x11, 0};
		ConnectionStatus = 0;
		SetTheECUToSpecificState(0xff);
		Activate_LEDPin(LED_CONNECT,OFF_MODE);
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_CONNECT , ret_frame , sizeof(ret_frame));
		
		#ifdef __RAM_MODE__
		Reset_Arm(0);
		#endif
		//return SCHEDULER_STATUS_END;
	}
}


void GeneralFaultProcedure(void)
{
		//find the fault task :
	int i =0;
	int r =0;
	int task_specific = 0;
			taskDiagRAM tskHelper;
	taskDiagRAM * tskPointer = &tskHelper;
	int res;
	
	
	for (i = 0; i < number_of_task ; i++)
	{
		if(task[i].type_task == (uint8_t)CMD2_TASK_DATA_FAULTS)
		{
			task_specific = i;
			break;
		}			
	}

	ConvertTask(task[task_specific], tskPointer);
	
	//res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
	//																															tskPointer->frmFormat[0].frm, 
	//																															tskPointer->frmFormat[0].frm_size,0);
	
	//m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
	

		for (r = 0; r < tskPointer->num_frame_send ; r++)
		{
			
			int res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																		tskPointer->frmFormat[r].frm,
																																		tskPointer->frmFormat[r].frm_size, r);
			
			char return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
			
			delay_ms(tskPointer->frmFormat[0].delay_between_frame);
			
			if(return_value == 1)
				r = r-1;
		}
		
	
	
	
	//if (RunTheSpecificTask(&task[task_specific]) != STATUS_ECU_CONNECTION_OKAY)
	//{
	//	GeneralConnectProcedure();
	//}
	
	task[task_specific].func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
	
		//if(res == STATUS_ECU_CONNECTION_OKAY)
		//delay_ms(tskPointer->frmFormat[0].delay_between_frame);

}



void GeneralActProcedure(void)
{
		//find the fault task :
	int i =0;
	int task_specific = 0;
	
	for (i = 0; i < number_of_task ; i++)
	{
		if(task[i].type_task == (uint8_t)CMD2_TASK_DATA_ACTUATOR)
		{
			task_specific = i;
			break;
		}			
	}
	
	m_Scheduler_Buffer.tskDiag = &task[task_specific];
	
	task[task_specific].func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
	
	
}



void SetTheECUToSpecificState(int num)
{
	if(num == 0xff)
	{
		m_Scheduler_Buffer.CMD = 0xff;
	}
	else
	{
		m_Scheduler_Buffer.CMD = cmd_mapping[num];
		m_Scheduler_Buffer.input_buf[0] = 0;
		m_Scheduler_Buffer.number_of_rec_byte = 0;			
	}
}



void ResetTheBuffer(void)
{
	m_Scheduler_Buffer.CMD = 0xff;
	m_Scheduler_Buffer.first_entering = 0;
}

char FindSpicificInputInArray(int test_number, const taskDiag task)
{
	int i=0;
	char flag = 0;
	for (i = 0; i < task.number_of_index ; i++)
	{
		if(task.index_array[i] == test_number)
		{
			flag = 1;
			break;
		}
	}
	
	return flag;
}


int FindTaskIndexForGivenStateInput(int indx, int cmd_type)
{
	int i=0;
	int flag = 0xffff;
	for ( i = 0; i < number_of_task ; i++)
	{
		if(task[i].type_task == cmd_type)
		{
				if(FindSpicificInputInArray(indx, task[i]))
				{
					flag =i;
					break;
				}
		}
	}
	return flag;
}

int FindTaskIndexForInput(int indx)
{
	int i=0;
	int flag = 0xffff;
	for ( i = 0; i < number_of_task ; i++)
	{
		if(FindSpicificInputInArray(indx, task[i]))
		{
			flag =i;
			break;
		}
	}
	return flag;
}

int checkSpecificDataInArray(int data, int * arrayData, int num)
{
	int i=0;
	int flag = 0xffff;
	for ( i = 0 ; i < num ; i++)
	{
		if(arrayData[i] == data)
		{
			flag = i;
			break;
		}
	}
	
	return flag;
}

int checkSpecificDataInArrayFromFlash(int data, const int * arrayData, int num)
{
	int i=0;
	int flag = 0xffff;
	for ( i = 0 ; i < num ; i++)
	{
		if(arrayData[i] == data)
		{
			flag = i;
			break;
		}
	}
	
	return flag;
}

int checkSpecificDataInCharArray(int data, int * arrayData, int num)
{
	int i =0;
	char flag = 0xffff;
	for (i = 0 ; i < num ; i++)
	{
		if(arrayData[i] == data)
		{
			flag = i;
			break;
		}
	}
	
	return flag;
}


uint8_t ConvertTask(const taskDiag tsk1, taskDiagRAM * tskDesc)
{
	int t=0;
	char * t1 = (char *)&tsk1;
	char * t2 = (char *)tskDesc;
	
	for (t = 0; t < sizeof(taskDiag) ; t++)
	{
		*t2++ = *t1++;
	}
	
	return 0;
}
extern uint8_t old_buf[512];

uint8_t reseTheMainBuffer(void)
{
	int i =0;
	for ( i = 0; i < NUMBER_OF_BUFFER_FOR_OUTPUT ; i++)
	{
		
		STRING_BUFFER * helper_destinition = &scheduler_output_buffer_final[i];
		helper_destinition->should_be_updated = 0;
		strcpy(helper_destinition->buf, "");
		helper_destinition->exceptional_condition = 0;
		helper_destinition->exceptional_value = 0;
		helper_destinition->enable_or_disable = 0;
	}
	
	memset(old_buf, 0xdd, 512);
}


char FindTheRequredConfigAndSendAndRecieve(char * input,int num) 
{
	int Item_Number = makeint(input[1], input[0]);
	int idx;
	//now find the Item_Number task
	//idx = FindTaskIndexForGivenStateInput(Item_Number, CMD2_TASK_DATA_CONFIGURATION);
	idx = FindTaskIndexForInput(Item_Number);
	
	if(idx != 0xffff)
	{
		taskDiagRAM tskHelper;
		taskDiagRAM * tskPointer = &tskHelper;
		
		ConvertTask(task[idx], tskPointer);
		
		m_Scheduler_Buffer.tskDiag = &task[idx];
		
		tskPointer->func_calculator(input,num);
		
	}
	else
	{
		//can find the item 
	}
	
}


extern uint8_t m_disableBufferStatus;
uint8_t m_startProcess=0;

char SendRecievedTheRequiredFrames(char * input,int num) 
{
	int number_of_task = 0;
	int task_index[10] = {0xff};
	int item_for_task[10][14] = {0xff};
	int nnn = 0;
	int ggg;
	int i,j,e,r = 0;
	int size_of_buffer;
	int num_1 = num /2;
	int index_cre = 0;
	int task_index_num[10] = {0};
	
	uint8_t * buffer_out;
	
	//construct the inputs :
	int input_items[14];
	
	for (i = 0 ; i < num /2 ;i++)
	{
		input_items[i] = makeint(input[index_cre++], input[index_cre++]);
	}
 
	
	 m_startProcess = 1;
	//finding the number of task!!!
	for (i = 0 ; i < num_1 ; i++)
	{
		int idx = FindTaskIndexForInput(input_items[i]);
		if(idx != 0xffff)
		{
			int return_value = checkSpecificDataInArray(idx, task_index, number_of_task); 
			
			if(return_value == 0xffff)
			{
				item_for_task[number_of_task][task_index_num[number_of_task]++] = input_items[i];
				task_index[number_of_task++] = idx;
			}
			else
			{
				item_for_task[return_value][task_index_num[return_value]++] = input_items[i];
			}
		}
	}
	
	//now you have map index!!!!
	
	for (ggg = 0; ggg < number_of_task ; ggg++)
	{

		
		taskDiagRAM tskHelper;
		taskDiagRAM * tskPointer = &tskHelper;
		
		ConvertTask(task[task_index[ggg]], tskPointer);
		
		for (r = 0; r < tskPointer->num_frame_send ; r++)
		{
			
			int res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																		tskPointer->frmFormat[r].frm,
																																		tskPointer->frmFormat[r].frm_size, r);
			
			char return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
			
			delay_ms(tskPointer->frmFormat[0].delay_between_frame);
			
			if(return_value == 1)
				r = r-1;
		}
		
			if(m_startProcess	== 0)
				return 0;
					
		
		tskPointer->func_calculator(input,num_1);
		
		//now you have send the frames and have results :
		for(e = 0; e < task_index_num[ggg] ; e++)
		{
			STRING_BUFFER * helper_com;
			STRING_BUFFER * helper_source;
			STRING_BUFFER * helper_destinition;
			
			int t =0;
			
			int idx_original_value = checkSpecificDataInCharArray(item_for_task[ggg][e], input_items, num_1);
			//int inx_original_value = 
			
			//find the index 
			for (t = 0; t< task_index_num[ggg] ; t++)
			{
				if(scheduler_output_buffer[t].item_num == item_for_task[ggg][e])
					break;
			}
			
			if(t == task_index_num[ggg])
			{
				return 0;
			}
			
			if(m_startProcess	== 0)
				return 0;
			
			helper_source = &scheduler_output_buffer[t];
			helper_destinition = &scheduler_output_buffer_final[idx_original_value];
			
			strcpy(helper_destinition->buf[0], helper_source->buf[0]);
			helper_destinition->enable_or_disable = helper_source->enable_or_disable;
			helper_destinition->exceptional_condition = helper_source->exceptional_condition;
			helper_destinition->exceptional_value = helper_source->exceptional_value;
			
			//the orginal one!!!
			helper_com = &scheduler_output_buffer_comparator[idx_original_value];
			if((strcmp(helper_destinition->buf[0], helper_com->buf[0]) == 0)	&& (m_disableBufferStatus == 0))
			{
				helper_destinition->should_be_updated = 0;
			}
			else
			{
				strcpy(helper_com->buf[0], helper_destinition->buf[0]);
				helper_destinition->should_be_updated = 1;
			}
		}
		
	}
	
	//now all the frames are occupied then send the frames:::
	//ruct the buffer and send them ...
	
	if(m_startProcess	== 0)
			return 0;
	
	for ( i = 0; i < num_1 ; i++)
	{
		
		STRING_BUFFER * helper_destinition = &scheduler_output_buffer_final[i];
		if(helper_destinition->should_be_updated == 1)
				nnn += (strlen(helper_destinition->buf)+3);
	}
	

	
	
	buffer_out = (uint8_t *)malloc(nnn+20);
	
	j = 0;
	buffer_out[j++] = m_ConnectionStatus;
	
	spi1_send_frame(XVBAT,m_Scheduler_Buffer.input_buf[1],0);
  spi1_recieve_frame();
          
  buffer_out[j++] =  xmegarx_frame[3];
	buffer_out[j++] =  xmegarx_frame[4];
	
	//buffer_out[j++] = 2;
	buffer_out[j++] = ecuStatusProgrammed[1];

				
	
	
	//type of the frame!!!
	
	for (i = 0; i < num_1 ; i++)
	{
		int y=0;
		
		STRING_BUFFER * helper_destinition = &scheduler_output_buffer_final[i];
		
		if(helper_destinition->should_be_updated == 1)
		{
			buffer_out[j++] = i;
			buffer_out[j++] = helper_destinition->enable_or_disable;
			buffer_out[j++] = helper_destinition->exceptional_value;
			buffer_out[j++] = strlen(helper_destinition->buf);
			for (y = 0; y < strlen(helper_destinition->buf) ; y++)
			{
				buffer_out[j++] = helper_destinition->buf[0][y];
			}					
		}

	}
	
	size_of_buffer = j;
	
	
	//now the data is ready you can send it to the android :
	if((size_of_buffer > 4) && (m_startProcess == 1) && (m_ConnectionStatus == CONNECTION_TO_CAR_OKAY))
	{
		WriteTheFrame(UART_1, CMD1_TASK_DATA, m_Scheduler_Buffer.CMD , buffer_out , size_of_buffer);
		m_disableBufferStatus = 0;
		m_startProcess = 0;
	}

	
	free(buffer_out);
	
	//if( m_Scheduler_Buffer.new_CMD != m_Scheduler_Buffer.CMD)
	//{
	//	m_Scheduler_Buffer.CMD = m_Scheduler_Buffer.new_CMD;
	//}		
	return 0;
}


char SendRecievedTheRequiredFramesForGraph(char * input,int num) 
{
	int number_of_task = 0;
	int task_index[10] = {0xff};
	int item_for_task[10][14] = {0xff};
	int nnn = 0;
	int ggg;
	int i,j,e,r = 0;
	int size_of_buffer;
	int num_1 = num /2;
	int index_cre = 0;
	int task_index_num[10] = {0};
	
	uint8_t * buffer_out;
	
	//construct the inputs :
	int input_items[14];
	
	for (i = 0 ; i < num /2 ;i++)
	{
		input_items[i] = makeint(input[index_cre++], input[index_cre++]);
	}
 
	
	 m_startProcess = 1;
	//finding the number of task!!!
	for (i = 0 ; i < num_1 ; i++)
	{
		int idx = FindTaskIndexForInput(input_items[i]);
		if(idx != 0xffff)
		{
			int return_value = checkSpecificDataInArray(idx, task_index, number_of_task); 
			
			if(return_value == 0xffff)
			{
				item_for_task[number_of_task][task_index_num[number_of_task]++] = input_items[i];
				task_index[number_of_task++] = idx;
			}
			else
			{
				item_for_task[return_value][task_index_num[return_value]++] = input_items[i];
			}
		}
	}
	
	//now you have map index!!!!
	
	for (ggg = 0; ggg < number_of_task ; ggg++)
	{

		
		taskDiagRAM tskHelper;
		taskDiagRAM * tskPointer = &tskHelper;
		
		ConvertTask(task[task_index[ggg]], tskPointer);
		
		for (r = 0; r < tskPointer->num_frame_send ; r++)
		{
			
			int res = m_CurrentProf->protocol_solver->SendAndRecieveFrame(m_CurrentProf->spect,
																																		tskPointer->frmFormat[r].frm,
																																		tskPointer->frmFormat[r].frm_size, r);
			
			char return_value = m_CurrentProf->protocol_solver->CheckTheResultsAndPerformAction(res);
			
			//delay_ms(tskPointer->frmFormat[0].delay_between_frame);
			delay_ms(100);
			
			if(return_value == 1)
				r = r-1;
		}
		
			if(m_startProcess	== 0)
				return 0;
					
		
		tskPointer->func_calculator(input,num_1);
		
		//now you have send the frames and have results :
		for(e = 0; e < task_index_num[ggg] ; e++)
		{
			STRING_BUFFER * helper_com;
			STRING_BUFFER * helper_source;
			STRING_BUFFER * helper_destinition;
			
			int t =0;
			
			int idx_original_value = checkSpecificDataInCharArray(item_for_task[ggg][e], input_items, num_1);
			//int inx_original_value = 
			
			//find the index 
			for (t = 0; t< task_index_num[ggg] ; t++)
			{
				if(scheduler_output_buffer[t].item_num == item_for_task[ggg][e])
					break;
			}
			
			if(t == task_index_num[ggg])
			{
				return 0;
			}
			
			if(m_startProcess	== 0)
				return 0;
			
			helper_source = &scheduler_output_buffer[t];
			helper_destinition = &scheduler_output_buffer_final[idx_original_value];
			
			strcpy(helper_destinition->buf[0], helper_source->buf[0]);
			helper_destinition->enable_or_disable = helper_source->enable_or_disable;
			helper_destinition->exceptional_condition = helper_source->exceptional_condition;
			helper_destinition->exceptional_value = helper_source->exceptional_value;
			
			//the orginal one!!!
			helper_com = &scheduler_output_buffer_comparator[idx_original_value];
			if((strcmp(helper_destinition->buf[0], helper_com->buf[0]) == 0)	&& (m_disableBufferStatus == 0))
			{
				helper_destinition->should_be_updated = 0;
			}
			else
			{
				strcpy(helper_com->buf[0], helper_destinition->buf[0]);
				helper_destinition->should_be_updated = 1;
			}
		}
		
	}
	
	//now all the frames are occupied then send the frames:::
	//ruct the buffer and send them ...
	
	if(m_startProcess	== 0)
			return 0;
	
	for ( i = 0; i < num_1 ; i++)
	{
		
		STRING_BUFFER * helper_destinition = &scheduler_output_buffer_final[i];
				nnn += (strlen(helper_destinition->buf)+3);
	}

	
	buffer_out = (uint8_t *)malloc(nnn+20);
	
	j = 0;
	buffer_out[j++] = m_ConnectionStatus;
	
          
  buffer_out[j++] =  0;
	buffer_out[j++] =  0;
	
	buffer_out[j++] = 2;
	
	
	//type of the frame!!!
	
	for (i = 0; i < num_1 ; i++)
	{
		int y=0;
		
		STRING_BUFFER * helper_destinition = &scheduler_output_buffer_final[i];
		
		//if(helper_destinition->should_be_updated == 1)
		{
			buffer_out[j++] = i;
			buffer_out[j++] = helper_destinition->enable_or_disable;
			buffer_out[j++] = helper_destinition->exceptional_value;
			buffer_out[j++] = strlen(helper_destinition->buf);
			for (y = 0; y < strlen(helper_destinition->buf) ; y++)
			{
				buffer_out[j++] = helper_destinition->buf[0][y];
			}					
		}

	}
	
	size_of_buffer = j;
	
	
	//now the data is ready you can send it to the android :
	if((size_of_buffer > 4) && (m_startProcess == 1) && (m_ConnectionStatus == CONNECTION_TO_CAR_OKAY))
	{
		WriteTheFrame(UART_1, CMD1_TASK_DATA, m_Scheduler_Buffer.CMD , buffer_out , size_of_buffer);
		m_disableBufferStatus = 0;
		m_startProcess = 0;
	}

	
	free(buffer_out);
	
	//if( m_Scheduler_Buffer.new_CMD != m_Scheduler_Buffer.CMD)
	//{
	//	m_Scheduler_Buffer.CMD = m_Scheduler_Buffer.new_CMD;
	//}		
	return 0;
}



extern char m_counter_fault;

void SendFaultFrame(uint8_t * buf, int size_buf, int number_of_Fault)
{
	int j =0;
	
	uint8_t * buffer_out = (uint8_t*)malloc(size_buf+ 8);
	
	j = 0;
	buffer_out[j++] = m_ConnectionStatus;
	
	spi1_send_frame(XVBAT,m_Scheduler_Buffer.input_buf[1],0);
  spi1_recieve_frame();
          
  buffer_out[j++] =  xmegarx_frame[3];
	buffer_out[j++] =  xmegarx_frame[4];
	
	
	buffer_out[j++] = ecuStatusProgrammed[0];
	
	
	if (ecuStatusProgrammed[0] == 0x60)
		buffer_out[j++] = number_of_Fault;
	else
		buffer_out[j++] = 0;
	
	
	if (ecuStatusProgrammed[0] == 0x60)
	{
	memcpy(buffer_out + 4, buf, size_buf);
	
	if((check_two_buffer(old_buf, buf, size_buf) != 0) && (m_ConnectionStatus == CONNECTION_TO_CAR_OKAY))
	{
		memcpy_P(buf, old_buf, size_buf);	
	
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_FAULTS , 
		buffer_out , size_buf + 4);		
	}		
	}
	else
	{
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_FAULTS , 
		buffer_out , size_buf + 5);		
	}

	
	/*
	if(m_counter_fault < 4)
	{
		memcpy_P(buf, old_buf, size_buf);	
		
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_FAULTS , 
		buffer_out , size_buf + 4);		
		m_counter_fault++;		
	} 
	else
	{
		
	if((check_two_buffer(old_buf, buf, size_buf) != 0))
	{
		memcpy_P(buf, old_buf, size_buf);	
	
		WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_FAULTS , 
		buffer_out , size_buf + 4);		
	}
  }
	
	*/
	
	free(buffer_out);
	}

	//WriteTheFrame(UART_1, CMD1_TASK_DATA, CMD2_TASK_DATA_ACTUATOR , ret_frame , sizeof(ret_frame));
void SendGeneralFrame(uint8_t * buf, int size_buf, int cmd1, int cmd2, int status1, int status2)
{
	int j =0;
	
	uint8_t * buffer_out = (uint8_t*)malloc(size_buf+ 8);
	
	j = 0;
	buffer_out[j++] = m_ConnectionStatus;
	
	spi1_send_frame(XVBAT,m_Scheduler_Buffer.input_buf[1],0);
  spi1_recieve_frame();
          
  buffer_out[j++] =  xmegarx_frame[3];
	buffer_out[j++] =  xmegarx_frame[4];
	
	buffer_out[j++] = status1;
	
	buffer_out[j++] = status2;
	
	memcpy(buffer_out + 4, buf, size_buf);
	
	WriteTheFrame(UART_1, cmd1, cmd2 , buffer_out , size_buf + 4);		

	free(buffer_out);
	}


void Runner(void)
{
	
	
	switch (m_Scheduler_Buffer.CMD)
	{
	case CMD2_TASK_DATA_CONNECT:
		m_Scheduler_Buffer.tskDiag = &task[CONNECT];
		m_Scheduler_Buffer.tskDiag->func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		
		break;
		
	case CMD2_TASK_DATA_SUMMARY:
		SendRecievedTheRequiredFrames(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
		
	case CMD2_TASK_DATA_PARAMETER:
		SendRecievedTheRequiredFrames(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
				
	case CMD2_TASK_DATA_CONFIGURATION:
		//SendRecievedTheRequiredFrames(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		FindTheRequredConfigAndSendAndRecieve(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
		
	case CMD2_TASK_DATA_IDENTIFICATION:
		SendRecievedTheRequiredFrames(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		
		break;
		
	case CMD2_TASK_DATA_FAULTS:
		GeneralFaultProcedure();
		break;
	
	case CMD2_TASK_DATA_ACTUATOR:
		//GeneralActProcedure();
	FindTheRequredConfigAndSendAndRecieve(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
	
		
	case CMD2_TASK_DATA_STANDBY:
		m_Scheduler_Buffer.tskDiag = &task[STANDBY];
		m_Scheduler_Buffer.tskDiag->func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
		
	case CMD2_TASK_DATA_DISCONNECT:
	#ifdef TEST_JUMPING
		Buzzer_On(100);	
		Buzzer_On(100);
		Buzzer_On(100);
	#endif 
	  
		Activate_LEDPin(LED_CONNECT,OFF_MODE);
		GeneralDisconnectProcedure(1);
    //m_ConnectionStatus = CONNECTION_TO_CAR_NO;
	m_Scheduler_Buffer.CMD = 0xff;
	
	#ifdef RAM_TEST
	Reset_Arm(10);
	#endif
	#ifdef __RAM_MODE__
		Reset_Arm(10);
	#endif
	  //m_jump_Condition = 1;
		//RunTheSpecificTask(&task[DISCONNECT]);
		
		//task[2].func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
	//jump to the bootloader :
	
		//__disable_irq();
		//NVIC_DeInit();
		//NVIC_SCBDeInit();
		//NVIC_SetVTOR(0);
		//boot_jump(0);
	
	
		break;

	case CMD2_TASK_DATA_ERASEFAULT:
		m_Scheduler_Buffer.tskDiag = &task[FAULT_ERASE];
		m_Scheduler_Buffer.tskDiag->func_calculator(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte); 
	break;
	
	case CMD2_TASK_DATA_GRAPH:
		//m_Scheduler_Buffer.graph_mode = 1;
		SendRecievedTheRequiredFramesForGraph(m_Scheduler_Buffer.input_buf, m_Scheduler_Buffer.number_of_rec_byte);
		break;
	

		
		
	default :
		break;
		
	}
}
