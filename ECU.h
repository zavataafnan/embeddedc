/* WARNING: Auto generated file. Do not modify. */
/*  Siemens Petrol Euro 2 */
#ifndef __E0001__
#define __E0001__

#include "ECU_global.h"

int number_of_task = 11;
void task_standby(char * input,int num); /* 0: frames 3e*/
void task_connect(char * input,int num); /* 1: frames 81*/
void task_disconnect(char * input,int num); /* 2: frames 82*/
void task_faultClean(char * input,int num); /* 3: frames 14,00,00*/
void task_parameter_2101(char * input,int num); /* 4: frames 21,01*/
void task_identification_1a8d(char * input,int num); /* 5: frames 1a,8d*/
void task_identification_1a8e(char * input,int num); /* 6: frames 1a,8e*/
void task_summary_18000(char * input,int num); /* 7: frames 18,0,0,0*/
void task_faults_18000(char * input,int num); /* 8: frames 18,00,00,00*/
void task_actuator_300007ff(char * input,int num); /* 9: frames 30,00,07,ff*/
void task_actuator_300004(char * input,int num); /* 10: frames 30,00,04*/

//task 0
const frameFormat frmFor_t0[] = {{{0x3e},1,0,3000}};
const int array_index_t0[] = {0};
//task 1
const frameFormat frmFor_t1[] = {{{0x81},1,0,300}};
const int array_index_t1[] = {1};
//task 2
const frameFormat frmFor_t2[] = {{{0x82},1,0,0}};
const int array_index_t2[] = {2};
//task 3
const frameFormat frmFor_t3[] = {{{0x14,0x00,0x00},3,0,300}};
const int array_index_t3[] = {3};
//task 4
const frameFormat frmFor_t4[] = {{{0x21,0x01},2,0,300}};
const int array_index_t4[] = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52};
//task 5
const frameFormat frmFor_t5[] = {{{0x1a,0x8d},2,0,300}};
const int array_index_t5[] = {53};
//task 6
const frameFormat frmFor_t6[] = {{{0x1a,0x8e},2,0,300}};
const int array_index_t6[] = {54};
//task 7
const frameFormat frmFor_t7[] = {{{0x18,0x0,0x0,0x0},4,0,100}};
const int array_index_t7[] = {55};
//task 8
const frameFormat frmFor_t8[] = {{{0x18,0x00,0x00,0x00},4,0,1500}};
const int array_index_t8[] = {56};
//task 9
const frameFormat frmFor_t9[] = {{{0x30,0x00,0x07,0xff},4,0,300}};
const int array_index_t9[] = {57,58,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,76,77,78};
const char array_data_chr_t9[] = {0,0x10,0x12,0x13,0x1B,0x1D,0x1C,0x20,0x24,0x31,0x32,0x15,0x16,0x3A,0x01,0x3B,0x02,0x3C,0x04,0x3D,0x08};
const TYPE_DATA_INTERNAL type_data_t9 = {array_data_chr_t9,NULL,NULL,NULL,NULL, 21};
//task 10
const frameFormat frmFor_t10[] = {{{0x30,0x00,0x04},3,0,300}};
const int array_index_t10[] = {70,79,80,81,82,83,84,85,86,87};
const char array_data_chr_t10[] = {0,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x59,0x5A};
const TYPE_DATA_INTERNAL type_data_t10 = {array_data_chr_t10,NULL,NULL,NULL,NULL, 10};

const taskDiag task[] = {
	{1, frmFor_t0, task_standby, 0, 1, array_index_t0, NULL} /* 0 */
	,{1, frmFor_t1, task_connect, 0, 1, array_index_t1, NULL} /* 1 */
	,{1, frmFor_t2, task_disconnect, 0, 1, array_index_t2, NULL} /* 2 */
	,{1, frmFor_t3, task_faultClean, 0, 1, array_index_t3, NULL} /* 3 */
	,{1, frmFor_t4, task_parameter_2101, 0x14, 49, array_index_t4, NULL} /* 4 */
	,{1, frmFor_t5, task_identification_1a8d, 0x12, 1, array_index_t5, NULL} /* 5 */
	,{1, frmFor_t6, task_identification_1a8e, 0x12, 1, array_index_t6, NULL} /* 6 */
	,{1, frmFor_t7, task_summary_18000, 0x11, 1, array_index_t7, NULL} /* 7 */
	,{1, frmFor_t8, task_faults_18000, 0x13, 1, array_index_t8, NULL} /* 8 */
	,{1, frmFor_t9, task_actuator_300007ff, 0x15, 21, array_index_t9, &type_data_t9} /* 9 */
	,{1, frmFor_t10, task_actuator_300004, 0x15, 10, array_index_t10, &type_data_t10} /* 10 */
};

#endif
/* WARNING: Auto generated file. Do not modify. */
