#ifndef __CRC_CALC__
#define __CRC_CALC__





#define FALSE	0
#define TRUE	!FALSE
#define POLYNOMIAL		0x1021
#define INITIAL_REMAINDER	0xFFFF
#define FINAL_XOR_VALUE		0xFFFF
#define REFLECT_DATA		TRUE
#define REFLECT_REMAINDER	TRUE
#define WIDTH    (8 * sizeof(unsigned short))
#define TOPBIT   (1 << (WIDTH - 1))

unsigned char reflect_byte(unsigned char data);
//void crcInit(void);
//unsigned short crcFast(unsigned char message[],char t);
#endif