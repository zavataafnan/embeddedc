#include "crc_calc.h"

unsigned short crcTable[256];

unsigned char reflect_byte(unsigned char data)
{
   unsigned char reflection = 0x00;
   unsigned char bit;

	/*
	 * Reflect the data about the center bit.
	 */
   for (bit = 0; bit < 8; ++bit)
   {
	/*
	 * If the LSB bit is set, set the reflection of it.
	 */
      if (((data & 0x01) > 0))
      {
	 reflection |= (unsigned char)(1 << (7 - bit));
      }

      data = (unsigned char)(data >> 1);
   }

   return (reflection);

}

unsigned short reflectushort_byte(unsigned short data)
{
   unsigned char a = reflect_byte((unsigned char)(data % 0x100));
   unsigned char b = reflect_byte((unsigned char)(data / 0x100));
   return ((unsigned short)(b * 0x100 + a));
}

void crcInit(void)
{
   unsigned short   remainder;
   unsigned long   dividend;
   unsigned char bit;
   
   /*
    * Compute the remainder of each possible dividend.
    */
   for (dividend = 0; dividend < 256; ++dividend)
   {
      /*
       * Start with the dividend followed by zeros.
       */
      remainder = dividend << (WIDTH - 8);

      /*
       * Perform modulo-2 division, a bit at a time.
       */
      for (bit = 8; bit > 0; --bit)
      {
	 /*
	  * Try to divide the current data bit.
	  */
	 if (remainder & TOPBIT)
	 {
	     remainder = (remainder << 1) ^ POLYNOMIAL;
	 }
	 else
	 {
	     remainder = (remainder << 1);
	 }
      }

      /*
       * Store the result into the table.
       */
      crcTable[dividend] = remainder;
   }
}
unsigned short crcFast(unsigned char message[], unsigned char t)
{
   unsigned short   remainder = INITIAL_REMAINDER;
   unsigned short   CRC,nBytes;
   unsigned char data;
	 unsigned short byteT;
   
   for (byteT = 0; byteT < t; ++byteT)
   {
      data = reflect_byte(message[byteT]) ^ (remainder >> (WIDTH - 8));
	    remainder = (crcTable[data]) ^ (remainder << 8);
   }

   /*
    * The final remainder is the CRC.
    */
   CRC = reflectushort_byte(remainder) ^ FINAL_XOR_VALUE;
   message[t]=CRC/0x100;
   message[t+1]=CRC;
   return CRC;
}	

/////crc fast megane//////
unsigned short crcFast_megane(char message[],char t)
{
   unsigned short   remainder = INITIAL_REMAINDER;
   unsigned short   CRC,byte,nBytes;
   unsigned char data;
   
   /*
    * Divide the message by the polynomial, a byte at a time.
    */
   if (message[0] == 0x10)
     nBytes = message[1];
   
   for (byte = t; byte < nBytes; ++byte)
   {
      data = reflect_byte(message[byte]) ^ (remainder >> (WIDTH - 8));
      remainder = crcTable[data] ^ (remainder << 8);
   }

   /*
    * The final remainder is the CRC.
    */
   CRC = reflectushort_byte(remainder) ^ FINAL_XOR_VALUE;
   message[nBytes]=CRC/0x100;
   message[nBytes+1]=CRC;
   return CRC;
}   /* crcFast() */


////////////////////////////////
///////  MUX CRC   /////////////
////////////////////////////////

unsigned short P_CCITT = 0x1021;
unsigned short u16PolyInvCCITT = 0x8408;
int CRC_TabCCITT_Init = 0;
static unsigned short CRC_TabCCITT [256];

void Init_CRCCCITT_Tab ()
{
 int i, j;
 unsigned short CRC, c;

 for (i = 0; i < 256; i++)
 {
  CRC = 0;
  c = ((unsigned short) i) << 8;
  for (j = 0; j < 8; j++)
  {
   if ((CRC ^ c) & 0x8000) CRC = (CRC << 1) ^ P_CCITT;//Polynomial
     else CRC <<= 1;
   c <<= 1;
  }
  CRC_TabCCITT[i] = CRC;
 }
 CRC_TabCCITT_Init = 1;
}

unsigned short u16CRC_Computation (unsigned char *pu8Buffer, unsigned short u16Size)
{
 unsigned char u08CounterBytes;
 unsigned char u08CounterBits;
 unsigned char u08BitValue;
 unsigned char BoolTest;
 unsigned short u16CRC;

 u16CRC = 0xFFFF;

 for (u08CounterBytes = 0; u08CounterBytes < u16Size; u08CounterBytes++)
 {
  for (u08CounterBits = 0; u08CounterBits < 8; u08CounterBits++)
  {
   u08BitValue = (pu8Buffer [u08CounterBytes] >> u08CounterBits) & 0x01;
   BoolTest = (u16CRC & 0x01) ^ u08BitValue;
   u16CRC >>= 1;
   if (BoolTest != 0) u16CRC ^= u16PolyInvCCITT;
  }
 }
 return ~u16CRC;
}

//int main(void)
unsigned int crcFast_MUX(unsigned char message[],char len)
{
  unsigned short CRC;
//  unsigned char Data [] = {
//    0x34,0x82,0x00,0x50,0x10,0xF0,
//    0xF7,0x01,0x02,0x02,0x09,0x00,0x00,0xFF,0x16,0x02,0x20,0x10,0x36,0xCB,0x02,
//    0x00,0x10,0x50,0x00,0x00,0x1B,0x50,0x00,0x00,0x40,0x50,0x00,0x00,0xFF,0xEF,
//    0x03,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//    0xFF,0xFF,0xFF,0x3F,0x06,0xAA,0x58,0x00,0x00,0x7F,0x00,0x80,0x07,0xE4,0xF3,
//    0x80,0x07,0xE0,0xF3,0x80,0x07,0xDC,0xF3,0x80,0x07,0xD8,0xF3,0x80,0x07,0xD4,
//    0xF3,0x80,0x07,0xD0,0xF3,0x80,0x07,0xCC,0xF3,0x80,0x07,0xC8,0xF3,0x80,0x07,
//    0xC4,0xF3,0x80,0x07,0x54,0x13,0x80,0x07,0xBC,0xF3,0x80,0x07,0xB8,0xF3,0x80,
//    0x07,0xB4,0xF3,0x80,0x07,0xB0,0xF3,0x80,0x07,0xAC,0xF3,0x80,0x07,0xA8,0xF3,
//    0x80,0x07,0xA4,0xF3,0x80,0x07,0xA0,0xF3,0x80,0x07,0x9C,0xF3,0x80,0x07,0x98,
//    0xF3,0x80,0x07,0x94,0xF3,0x80,0x07,0x90,0xF3,0x80,0x07,0xD4,0x14,0x80,0x07,
//    0x88,0xF3,0x80,0x07,0x84,0xF3,0x80,0x07,0xF4,0x15,0x80,0x07,0x7C,0xF3,0x80,
//    0x07,0x78,0xF3,0x80,0x07,0x94,0x12,0x80,0x07,0x70,0xF3,0x80,0x07,0x6C,0xF3,
//    0x80,0x07,0x60,0x12,0x80,0x07,0x64,0xF3,0x80,0x07,0x60,0xF3,0x80,0x07,0x2C,
//    0x12,0x80,0x07,0x58,0xF3,0x80,0x07,0x18,0x14,0x80,0x07,0x50,0xF3,0x80,0x07,
//    0x4C,0xF3,0x80,0x07,0x48,0xF3,0x80,0x07,0x44,0xF3,0x80,0x07,0x40,0xF3,0x80,
//    0x07,0x3C,0xF3,0x80,0x07,0x38,0xF3,0x80,0x07,0x34,0xF3,0x80,0x07,0x30,0xF3,
//    0x00,0x00
//  };

//  Init_CRCCCITT_Tab ();
  CRC = u16CRC_Computation (message, len);
  message[len] = CRC%0x100;
  message[len + 1] = CRC/0x100;
  return CRC;

//  printf ("\n%2.2x%2.2x", CRC%0x100, CRC/0x100);
}

/*
unsigned char Key1;
unsigned long int Key2;
unsigned long int Key3;
unsigned long int Result;

int KWP2000_Calc(char a1, int a2) {
	unsigned char v4;
	unsigned char v6;

	v6 = a1 + 0x23;
	if (v6 < 0x23)
		v6 = 0xFF;
	v4 = 0;
	while (v4 < v6) {
		if (a2 & 0x80000000)
			a2 = Key3 ^ (2 * a2);
		else
			a2 *= 2;
		v4++;
	}
	return a2;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// *******************************
void Init30R30_32(unsigned long POLYNOMIAL_T) {
	unsigned long v2;
	unsigned long i, j;

	for (i = 0; i <= 255; i++) {
		v2 = i;
		for (j = 0; j < 8; j++) {
			if ((v2 & 0x0001) == 0x0001)
				v2 = (v2 >> 1) ^ POLYNOMIAL_T;
			else
				v2 >>= 1;
		}
		crcTable[i] = v2;
	}
}

// *******************************
*/