#include "EEPROMManage.h"
#include "lpc177x_8x_eeprom.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_rtc.h"
#include "stdlib.h"
#include "blowfish.h"

extern SECURITY_ID m_SecurityID;

#define PAGE_OFFSET			0x00
#define PAGE_ADDR			0x01


extern uint8_t key3_alberto[];
BLOWFISH_KEY key;


#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=4
uint8_t read_buffer_eeprom[EEPROM_PAGE_SIZE*2];
//#pragma data_alignment=4
//uint8_t write_buffer[]="NXP Semiconductor LPC177x_8x-CortexM3 \n\r\t--- HELLO WORLD!!! ---";
#else
uint8_t __attribute__ ((aligned (4))) read_buffer_eeprom[EEPROM_PAGE_SIZE*2];
//uint8_t __attribute__ ((aligned (4))) write_buffer_from_eeprom[EEPROM_PAGE_SIZE*2]="";
#endif

//uint8_t read_bufferA[64][64];
unsigned char EraseTheEEPROMAndFillWithRandom(void)
{
	int i,j=0;
	RTC_TIME_Type RTCFullTime;
	
    for( i = 0; i < EEPROM_PAGE_NUM; i++)
    {
        //EEPROM_Erase(i);
    }
    
    
    RTC_GetFullTime (LPC_RTC, &RTCFullTime);
  
  

    
    for(i = 0; i < EEPROM_PAGE_NUM; i++)
    {
      uint8_t buf[64];
      RTC_GetFullTime (LPC_RTC, &RTCFullTime);
      srand((unsigned) RTCFullTime.SEC * (i+1));
      
      for (j = 0 ; j < 64 ; j++)
        buf[j]  = rand();
      
      EEPROM_Write(0,i,(void*)buf,MODE_8_BIT,64);
    }
    
    
    for(i = 0; i < EEPROM_PAGE_NUM; i++)
    {
      //EEPROM_Read(0,i,(void*)read_bufferA[i],MODE_8_BIT,64);
    }
    
    return 0;
    
}


unsigned char * Encrypted_SecurityID(int * size_buffer)
{
	
	uint8_t * security_buf = (uint8_t*)&(m_SecurityID);
	
	uint8_t * buffer;
	int output_file=0;
	int i =  0;
	int exact_8_18_size = ((sizeof(SECURITY_ID)+1) / 8 + 1) * 8;
	RTC_TIME_Type RTCFullTime;
	
	
	buffer = (uint8_t *)malloc(exact_8_18_size);
	memcpy(buffer+1, security_buf, sizeof(SECURITY_ID));
	
	
  srand((unsigned) RTCFullTime.SEC);
	//1 crypted_frame
	buffer[0] = rand() % 255;
	
	for (i = 1; i < sizeof(SECURITY_ID) + 1 ; i++)
		buffer[i] ^= buffer[0];
	
	
	key_schedule(key3_alberto, &key, 24);
	

	for (i = 0; i < exact_8_18_size / 8; i++)
	{
		uint8_t buf_8bytes[8];
		uint8_t buf_crypted8bytes[8];

		memcpy(buf_8bytes, buffer + i * 8 + 1, 8);

		blowfish_encrypt(buf_8bytes, buf_crypted8bytes, &key);

		//blowfish_decrypt(c, p3, &key);

		memcpy(buffer + i * 8 + 1, buf_crypted8bytes, 8);
	}
	
	*size_buffer = exact_8_18_size;
	return buffer;
}


SECURITY_ID * Decrypted_SecurityID(unsigned char * buffer)
{
	
	SECURITY_ID * sec_id; 
	uint8_t * security_buf = (uint8_t*)&(m_SecurityID);
	
	int output_file=0;
	int i =  0;
	int exact_8_18_size = ((sizeof(SECURITY_ID)+1) / 8 + 1) * 8;
	RTC_TIME_Type RTCFullTime;
	
	
	
	key_schedule(key3_alberto, &key, 24);
	

	for (i = 0; i < exact_8_18_size / 8; i++)
	{
		uint8_t buf_8bytes[8];
		uint8_t buf_crypted8bytes[8];

		memcpy(buf_8bytes, buffer + i * 8 + 1, 8);

		blowfish_decrypt(buf_8bytes, buf_crypted8bytes, &key);

		memcpy(buffer + i * 8 + 1, buf_crypted8bytes, 8);
	}
	
	for (i = 1; i < sizeof(SECURITY_ID) + 1 ; i++)
		buffer[i] ^= buffer[0];
	
	
	sec_id = (SECURITY_ID *) (buffer+1);
	
	return sec_id;
}


/*
unsigned char WriteTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
  
  uint8_t * security_buf = (uint8_t*)(security_id);
	uint8_t * buffer;
	int output_file=0;
	int exact_size = (sizeof(SECURITY_ID) / 16 + 1) * 16;
	int exact_18_size = (exact_size / 16) * 18;
	int exact_8_18_size = (exact_18_size / 8 + 1) * 8;
	
	buffer = (uint8_t *)malloc(exact_8_18_size+ 100);
	
	Encrypt_Special_Structure(security_buf, sizeof(SECURITY_ID), buffer, &output_file);
	
	EEPROM_Write(PAGE_OFFSET,PAGE_ADDR,(void*)buffer,MODE_8_BIT,output_file);
	
	free(buffer);
	
  return 0;
}

unsigned char ReadTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
  
  uint8_t * buf = (uint8_t*)(security_id);
	uint8_t * crypted_buf;
	int output_file1 =0 ;
	
	int exact_size = (sizeof(SECURITY_ID) / 16 + 1) * 16;
	int exact_18_size = (exact_size / 16) * 18;
	int exact_8_18_size = (exact_18_size / 8 + 1) * 8;
	
	
	crypted_buf = (uint8_t *)malloc(exact_8_18_size + 100);
	
  EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)crypted_buf,MODE_8_BIT,exact_8_18_size);
	
	Decrypt_Special_Structure(crypted_buf, exact_8_18_size,buf , &output_file1);
  
  //SECURITY_ID * test = (SECURITY_ID *)read_buffer_eeprom;
  free(crypted_buf);
  
  return 0;
}
*/


/*
unsigned char WriteTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
  
  uint8_t * buf = (uint8_t*)(security_id);
  
  //_DBG_("Write data to EEPROM");
  EEPROM_Write(PAGE_OFFSET,PAGE_ADDR,(void*)buf,MODE_8_BIT,sizeof(SECURITY_ID)/1);
  //_DBG_("Read data from EEPROM");
  //EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)read_buffer_eeprom,MODE_8_BIT,sizeof(SECURITY_ID)/1);
  
  //SECURITY_ID * test = (SECURITY_ID *)read_buffer_eeprom;
  
  
  return 0;
}

unsigned char ReadTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
  
  uint8_t * buf = (uint8_t*)(security_id);
  
  //_DBG_("Write data to EEPROM");
  //_DBG_("Read data from EEPROM");
  EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)buf,MODE_8_BIT,sizeof(SECURITY_ID)/1);
  
  //SECURITY_ID * test = (SECURITY_ID *)read_buffer_eeprom;
  
  
  return 0;
}
*/

unsigned char WriteTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
  int size_buffer = 0;
  uint8_t * buf = Encrypted_SecurityID(&size_buffer);
	
  EEPROM_Write(PAGE_OFFSET,PAGE_ADDR,(void*)buf,MODE_8_BIT,size_buffer);
  
	free(buf);
  
  return 0;
}

unsigned char ReadTheSecurityPatchToEEPROM(SECURITY_ID * security_id)
{
	
  uint8_t * sec_id = (uint8_t *) security_id; 
  uint8_t * buf;
	int exact_8_18_size = ((sizeof(SECURITY_ID)+1) / 8 + 1) * 8;
	buf = (uint8_t *)malloc(exact_8_18_size);
	
  EEPROM_Read(PAGE_OFFSET,PAGE_ADDR,(void*)buf,MODE_8_BIT,exact_8_18_size);
	Decrypted_SecurityID(buf);
	
	memcpy(sec_id, buf+1, sizeof(SECURITY_ID));
	
	free(buf);

  return 0;
}




