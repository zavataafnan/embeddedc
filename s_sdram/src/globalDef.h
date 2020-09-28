#ifndef __GLOBAL_DEF_
#define __GLOBAL_DEF_

#include "stdint.h"
#include "lpc_types.h"

#define MACHINE_ADDRESS  0x01

//--------------------------------------------- Firmware Version Info
#define	FIRMWARE_VERSION_YEAR			94
#define	FIRMWARE_VERSION_MONTH		05
#define	FIRMWARE_VERSION_DAY			05
//---------------------------------------------

#define BOOTLOADER_CODE  					0x20;

#define CTLTXD2_1		(uint32_t)(1<<9)
#define CTLTXD2_2		(uint32_t)(1<<8)
#define CTLTXD2_3		(uint32_t)(1<<24)
#define CTLTXD2_4		(uint32_t)(1<<25)

//#define CTL485_0	(uint32_t)(1<<22)
#define CTL485_1		(uint32_t)(1<<26)		//P1.25 (LED2)
#define CTL485_2		(uint32_t)(1<<25)

#define E_4052			(uint32_t)(1<<27)	

 #define MAX_DEV_CNT	9  // Max Device Count
 #define MAX_TAG_CNT	90 // Max Tag Count
 
//typedef Bool 						BOOL;
typedef unsigned char 	BYTE;
typedef unsigned char 	UCHAR;

//typedef char						CHAR;

typedef unsigned short	USHORT;
typedef signed short		SHORT;

#define setbit(address,bit) (address |= (1<<bit))
#define clearbit(address,bit) (address &= ~(1<<bit))

 typedef union __int_to_char__
 {
          uint8_t ai8[2];
	 uint16_t i16;	 
 } int_char_little;
 
 typedef union __int32_to_char__
 {
	 uint8_t ai8[4];
	 uint32_t i32;
 } INT32_TO_CHAR;
 
#endif
/////
/////
/////
 