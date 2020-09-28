#include "CRCGenerator.h"
#include "CommProtocol.h"
#include "debug_frmwrk.h"


  /*******************************************************************\
    *                                                                   *
    *   unsigned short update_crc_ccitt( unsigned long crc, char c );   *
    *                                                                   *
    *   The function update_crc_ccitt calculates  a  new  CRC-CCITT     *
    *   value  based  on the previous value of the CRC and the next     *
    *   byte of the data to be checked.                                 *
    *                                                                   *
    \*******************************************************************/

    /*******************************************************************\
    *                                                                   *
    *   static void init_crcccitt_tab( void );                          *
    *                                                                   *
    *   The function init_crcccitt_tab() is used to fill the  array     *
    *   for calculation of the CRC-CCITT with values.                   *
    *                                                                   *
    \*******************************************************************/

  extern m_FrameSettingCmdStructure m_frmSettingCmd;
  
uint8_t isCheckSumCorrect(uint8_t * tx)
{
  uint32_t result, i;
  uint16_t crc_ccitt_0000 = 0x8409;
  int_char_little conCharToInt;
	int_char_little conCharToIntCalc;
	char val_t = 0;
	uint16_t size_buf = 0;
	char buffera[30];
  
  m_FrameCmdStructure * frmCmdStruct = (m_FrameCmdStructure *)tx;
  uint16_t size_of_buffer = frmCmdStruct->m_iLen - sizeof(m_FrameCmdStructure) -1  - 2;
  
  conCharToInt.ai8[0] = tx[frmCmdStruct->m_iLen-1];
  conCharToInt.ai8[1] = tx[frmCmdStruct->m_iLen-2];
  
	
	    
    
    do {

        unsigned char val_t = (unsigned char)tx[size_buf];
			  //sprintf(buffera, "%X,%d \r\n", crc_ccitt_0000, val_t);
				//_DBG(buffera);
        crc_ccitt_0000 = crcByte(crc_ccitt_0000, val_t);
        size_buf++;

    } while (size_buf < frmCmdStruct->m_iLen - 2);

    conCharToIntCalc.ai8[0] = crc_ccitt_0000 & 0xff;
    conCharToIntCalc.ai8[1] = (crc_ccitt_0000 >> 8) & 0xff;
		
		
		
  if(m_frmSettingCmd.m_ucCRCType == 0x80)
  {
    return 0;
  }
  else
  {
    if(conCharToInt.i16 == conCharToIntCalc.i16)
      return 0;
    else
      return 1;    
  }  
}

/*
uint16_t   crc_tabccitt[256];
uint16_t              crc_tabccitt_init       = FALSE;

#define                 P_16        0xA001
#define                 P_32        0xEDB88320L
#define                 P_CCITT     0x1021
#define                 P_DNP       0xA6BC
#define                 P_KERMIT    0x8408
#define                 P_SICK      0x8005


void init_crcccitt_tab( void ) {

    int i, j;
    uint16_t crc, c;

    for (i=0; i<256; i++) {

        crc = 0;
        c   = ((uint16_t) i) << 8;

        for (j=0; j<8; j++) {

            if ( (crc ^ c) & 0x8000 ) crc = ( crc << 1 ) ^ P_CCITT;
            else                      crc =   crc << 1;

            c = c << 1;
        }

        crc_tabccitt[i] = crc;
    }

    crc_tabccitt_init = TRUE;

}  


uint16_t update_crc_ccitt( unsigned short crc, char c ) {

    uint16_t tmp, short_c;

    short_c  = 0x00ff & (uint16_t) c;

    if ( ! crc_tabccitt_init ) init_crcccitt_tab();

    tmp = (crc >> 8) ^ short_c;
    crc = (crc << 8) ^ crc_tabccitt[tmp];

    return crc;

}  


    
    int size_buf = 0;
    result = 0x8409;
    do {
      
      char val_t = (char)tx[size_buf];
      result = update_crc_ccitt(result, val_t);
      
      size_buf++;
      
    }
    while (size_buf < frmCmdStruct->m_iLen-2);
    */

