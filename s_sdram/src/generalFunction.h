#pragma once
#include "stdint.h"

char checkbiter(char address,char bit);
unsigned int makeint(char high,char low);
signed int makesignint(char high,char low);
signed char makesignchar(char Input);
char checkIdInArray(int idx);
void array_copy( char copy[],char c_start,char c_end,char main[],char m_start,char m_end);
void HexConvert(char * buf_hex, char * buf, char num); 
char CheckInFrme(int * input, int tskId, char tskNum);
void memcpy_P(const uint8_t * src, uint8_t * dest , int num);
void Reset_Arm(uint32_t value);
void Buzzer_On(int time);