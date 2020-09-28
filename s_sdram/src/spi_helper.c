#include "spi_helper.h"
#include "lpc_types.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "system_lpc177x_8x.h"
#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "debug_frmwrk.h"
#include "diskio.h"
#include "ff.h"
#include "board.h"
#include "lpc177x_8x_eeprom.h"
#include "SecurityLevels.h"
#include "Protocol_CAN.h"
#include "CarCommProtocol.h"
#include "sdram.h"
#include "RelayController.h"


//////////////////
//ssp variables///
//////////////////
  #define _USING_SSP		(LPC_SSP1)

  // PORT number that /CS pin assigned on
  #define CS_PORT_NUM             0
  
  // PIN number that  /CS pin assigned on
  #define CS_PIN_NUM              6

// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;

//start, length, type, reserved, check_sum
unsigned char xmegatx_frame[256];
unsigned char xmegarx_frame[256];

//end ssp section

static FATFS lFatfs[1];
static FILINFO Finfo;


void slave_enable()
{
 GPIO_ClearValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
}

void slave_disable()
{
GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
}







//Boot functions can only be executed if system is in boot mode
void spi1_send_frame(unsigned char action, unsigned char param1, unsigned char param2)
{
        char counter;
        char chksum=0;
        //unsigned char xmegatx_frame[] = {0x80,0x05,0x02,0x01,0x88};
        //start byte
        xmegatx_frame[0] = 0x80;
        xmegatx_frame[1] = 0x05;
        xmegatx_frame[2] = action;
        xmegatx_frame[3] = param1;        
        
        switch(action) {
         
       
         case XOSCOPE :
         xmegatx_frame[1] = 0x06;
         xmegatx_frame[4] = param2;
         break;
        

         case XBOOT_SEND_ADDRESS :
         xmegatx_frame[1] = 0x06;
         xmegatx_frame[4] = param2;         
         break;

         case XBOOT_SEND_128FLASH_bytes :
         xmegatx_frame[1] = 133;
         break;
         
         
         
        

       
        default :
         //do nothing
          break;
         }
       
        //create checksum
        for(counter=0;counter<xmegatx_frame[1]-1;counter++)
          chksum+=xmegatx_frame[counter];
        
        xmegatx_frame[xmegatx_frame[1]-1] = chksum;
        
        //enable ssp1
        slave_enable();
        
        for(counter=0;counter<xmegatx_frame[1];counter++)
        {
          SSP_SendData(_USING_SSP, xmegatx_frame[counter]);
          
          //while FIFO not full continue flling it up
          while(!SSP_GetStatus(_USING_SSP,SSP_STAT_TXFIFO_NOTFULL));
             // delay_ms(1);

        }
          //wait for data to finish sending
          while(!SSP_GetStatus(_USING_SSP,SSP_STAT_TXFIFO_EMPTY));

        //disable ssp1
        //slave_disable();
}

// 0x00 success
// 0x01 timeout
// 0x02 chksum error
// 0x03 Xmega recieved an unsupported frame
unsigned char spi1_recieve_frame(void)
{
        unsigned char counter;
        unsigned char chksum=0;
        uint16_t spi16data=0;
        unsigned char rx_data=0;
        unsigned int timeout_counter;
        
        
    
        //enable ssp1
        slave_enable();
        
        //flush recive buffer
        while(SSP_GetStatus(_USING_SSP,SSP_STAT_RXFIFO_NOTEMPTY))
          SSP_ReceiveData(_USING_SSP);

          //send one dummy to initiate transmission
           //data read after this is invalid, discard it
          SSP_SendData(_USING_SSP, 0xff);
          //wait for data to arrive and discard
          while(!SSP_GetStatus(_USING_SSP,SSP_STAT_RXFIFO_NOTEMPTY));
          rx_data = SSP_ReceiveData(_USING_SSP);
          
          //SSP_ReceiveData(_USING_SSP);
          //delay_ms(1);
 
          
          rx_data=0;
          timeout_counter = 50000;
          while(rx_data!=0x81 && timeout_counter>0)
          {
         
           //send dummy to read data
          SSP_SendData(_USING_SSP, 0xff);

           //wait here until recieve complete
           while(!SSP_GetStatus(_USING_SSP,SSP_STAT_RXFIFO_NOTEMPTY));
           //read recived data       
           rx_data = SSP_ReceiveData(_USING_SSP);
           xmegarx_frame[0] = rx_data;
           timeout_counter--;
          }
          if(timeout_counter==0)
          {
           return(1);
          }

          
          chksum = 0x81;
          //recieve length byte
          SSP_SendData(_USING_SSP, 0xff);
            
            //wait here until recieve complete
            while(!SSP_GetStatus(_USING_SSP,SSP_STAT_RXFIFO_NOTEMPTY));
            //read recived data       
            //delay_ms(1);
            rx_data = SSP_ReceiveData(_USING_SSP);
            xmegarx_frame[1] = rx_data;
            chksum+=rx_data;

          for(counter=2;counter<xmegarx_frame[1];counter++)
          {
          //              delay_ms(1);

            SSP_SendData(_USING_SSP, 0xff);
            
            //wait here until recieve complete
            while(!SSP_GetStatus(_USING_SSP,SSP_STAT_RXFIFO_NOTEMPTY));
            //read recived data       
            //delay_ms(1);
            rx_data = SSP_ReceiveData(_USING_SSP);
            xmegarx_frame[counter] = rx_data;
            if(counter<xmegarx_frame[1]-1)
            {
              chksum+=rx_data;
            }
          }
          
          if(chksum==rx_data)
          {
            if(xmegarx_frame[2]==0xff)
            {
             //Xmega recieved unsupported frame
             return(0x03);
            }
            else
            {
             //frame ok
             return(0);
            }
          }
          else if(chksum!=rx_data)
          {
            //checksum error
            return(2);
          }
          
}




void SPI_initialize_internal(void)
{
  
  //////////////////////
  //spi code starts here
  //////////////////////
  
  
  //Define SPI_1 pins configuration
  
  
  ///*************************
  // Arm hamishe bayad frame ke ba 0x80 shoroo mishwawd
  // ra be soorat kamel beferestad
  // Dar gheire in soorat system xmega momken ast gahti konad
  // be khosoos agar wastat kar ham yek 0x80 bashad
  // baraye bootloader ham bayad tabe jadid tarif shawad ke polling bahad wa
  //interrupt ha gheire fa'al
  ///*************************
  
  
  
  
  //set SS as GPIO for enabl/disabling slave
  PINSEL_ConfigPin(0, 6, 0);
  //output
  GPIO_SetDir(CS_PORT_NUM, (1<<CS_PIN_NUM), 1);
  
  
  //value 1 (slave disabled)
  GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
  
  
  //value 1 (slave disabled)
  GPIO_ClearValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
  
  
  //rest of pins as normal spi1
  PINSEL_ConfigPin(0, 7, 2);
  PINSEL_SetFilter(0, 7, 0);
  
  PINSEL_ConfigPin(0, 8, 2);
  PINSEL_SetFilter(0, 8, 0);
  
  PINSEL_ConfigPin(0, 9, 2);
  PINSEL_SetFilter(0, 9, 0);
  
  
  
  /* Initializing SSP device section ------------------------------------------------------ */
  // initialize SSP configuration structure to default
  /*
  SSP_InitStruct->CPHA = SSP_CPHA_FIRST;
  SSP_InitStruct->CPOL = SSP_CPOL_HI;
  SSP_InitStruct->ClockRate = 1000000;
  SSP_InitStruct->Databit = SSP_DATABIT_8;
  SSP_InitStruct->Mode = SSP_MASTER_MODE;
  SSP_InitStruct->FrameFormat = SSP_FRAME_SPI;
  */
  
  SSP_ConfigStructInit(&SSP_ConfigStruct);
  SSP_ConfigStruct.ClockRate = 200000;
  
  // Initialize SSP peripheral with parameter given in structure above
  SSP_Init(_USING_SSP, &SSP_ConfigStruct);
  
  // Enable SSP peripheral
  SSP_Cmd(_USING_SSP, ENABLE);
  
}

//ok:
// 0x10 normal mode
// 0x11 boot mode

//error:
// 0x01 timeout
// 0x02 chksum error
// 0x03 Xmega recieved an unsupported frame
unsigned char get_boot_status(void)
{
  unsigned char status;
  spi1_send_frame(XGET_BOOT_STATUS,0,0);
  status = spi1_recieve_frame();
  //if frame ok
  if(status==0)
  {
   return(xmegarx_frame[3]);
  }
  //if error
  else return(status);
}


// 0x00 success
// 0x01 timeout
// 0x02 chksum error
// 0x03 Xmega recieved an unsupported frame
//
//Formula: Threshold voltage = 3.3 * (scale_factor + 1) *8/64 = 0.4125(scale_factor + 1)
unsigned char set_comparator_threshold(unsigned char scale_factor)
{
   spi1_send_frame(XSET_THRESHOLD,scale_factor,0);
   return(spi1_recieve_frame());
}


//00 success
// 0x01 timeout
// 0x02 chksum error
// 0x03 Xmega recieved an unsupported frame
//data is sent in 128 byte packets. At beginning of every 256 bytes an address byte is sent
//xmega integrates each two 128 packets in a 256 byte buffer (page size of xmega flash buffer)
unsigned char write_flash(unsigned char *boot_code_buffer,unsigned int length)
{
    unsigned int flash_code_size = length; //flash size in bytes
    unsigned char segment;
    unsigned char temp_padding_buffer[256] = {0xff};
    unsigned int counter;
    unsigned char F_stat;

    //boot code must be a factor of 256, if it isn't code should be padded with zero
    //if not  a factor of 256 do following procedure for padding
    unsigned char quotient = flash_code_size/256;
    unsigned int adress_of_block_needing_padding = quotient * 256;
    
    for(counter=0; counter<adress_of_block_needing_padding; counter+=128)
    {
     //send adress word
     if(counter%256 == 0)
     {
      //Action, LSB of address, MSB of address
      spi1_send_frame(XBOOT_SEND_ADDRESS,(unsigned char)(counter & 0x00FF),(unsigned char)(counter >> 8));
      F_stat = spi1_recieve_frame();
      if(F_stat>0)
      {
       return(F_stat);
      }
      segment = 0;
     }
     else
     {
     segment =1;
     }
     
     //copy relevant flash section to transmit buffer
     memcpy(&xmegatx_frame[4],&boot_code_buffer[counter],128);
     spi1_send_frame(XBOOT_SEND_128FLASH_bytes,segment,0);
     F_stat = spi1_recieve_frame();
      if(F_stat>0)
      {
       return(F_stat);
      }

    }
    
    //if last segment remaining that needs padding
    if(flash_code_size%256 != 0)
    {
      //Action, LSB of address, MSB of address
      spi1_send_frame(XBOOT_SEND_ADDRESS,(unsigned char)(counter & 0x00FF),(unsigned char)(counter >> 8));
      F_stat = spi1_recieve_frame();
      if(F_stat>0)
      {
       return(F_stat);
      }
      segment = 0;
      //copy relevant flash section to padding buffer
      memcpy(&temp_padding_buffer,&boot_code_buffer[counter],flash_code_size%256);
      
      //
      memcpy(&xmegatx_frame[4],&temp_padding_buffer[0],128);
      spi1_send_frame(XBOOT_SEND_128FLASH_bytes,segment,0);
      F_stat = spi1_recieve_frame();
      if(F_stat>0)
      {
       return(F_stat);
      }
      
      segment = 1;
      memcpy(&xmegatx_frame[4],&temp_padding_buffer[128],128);
      spi1_send_frame(XBOOT_SEND_128FLASH_bytes,segment,0);
      F_stat = spi1_recieve_frame();
      if(F_stat>0)
      {
       return(F_stat);
      }
    }
 
 //return 0 successs
 return(0);

////place other return values here for CRC  and etc.
}


//At initial programming both app and boot section are erased by programmer. Then app and boot code are programmed into system (make sure
//erase chip is not selected when programming these sections).
//Then device is completely disconnected from all sources of power and is turned on. Boot fuse bit is also burned into system.
//This causes the system to write the EEPROM boot code which starts the system in app mode. Then the following subroutine is executed.
// 0x00 succesful
// 0x01 timeout
// 0x02 chksum error
// 0x03 Xmega recieved an unsupported frame
// 0x10 XMEGA cannot enter boot mode
unsigned char program_application_using_bootloader(unsigned char *boot_code_buffer,unsigned int length)
{

    //first check if system is already in bootloader mode
    unsigned char boot_status = get_boot_status();
    unsigned char F_stat;
    //if in normal mode
    if(boot_status==XMEGA_MODE_NORMAL)
    {
      //enter bootmode (sets EEPROM boot flags to bootloader and initializes software reset)
      spi1_send_frame(XBOOT_ENTER,0,0);
      // no response is given to this frame
      
      //wait for system to enter bootloader mode
      delay_ms(200);
      
      boot_status = get_boot_status();
      if(boot_status != XMEGA_MODE_BOOT)
      {
      //cannot enter boot-mode  
      return(boot_status);
      }
    }
    //if in boot mode
    if(XMEGA_MODE_BOOT==XMEGA_MODE_BOOT)
    {
      //erase application section
      spi1_send_frame(XBOOT_ERASE_64K_FLASH,0,0);
      F_stat = spi1_recieve_frame();
      //if not ok
      if(F_stat!=0)
      {
        return(F_stat);
      }
            
      ///write binary application code to XMEGA flash memory
      F_stat =  write_flash(boot_code_buffer,length);
      //if not ok
      if(F_stat!=0)
      {
        return(F_stat);
      }
      
      //end boot mode and initate software reset for systemt to enter app mode
      spi1_send_frame(XBOOT_END_MODE,0,0);
      // no response is given to this frame
      
      //wait for system startup in application mode
      delay_ms(200);
      
      return(0); //succesfuly programmed system (CRC check might be needed here)
    }
  //if boot status is neither boot nor normal
  return(boot_status);
}


unsigned char program_in_bootmode_from_file(void)
{
  DIR dir;
  BYTE res;
  FIL file;
  UINT numRead = 0;
  UINT numWritten = 0;
  TIM_TIMERCFG_Type timerCfg;
	void * byte_wr_ptr;
  unsigned char tt;
  
  uint8_t buff[512];
  // initialize timer
  TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);
  
  console_init();
  
  SysTick_Config(SystemCoreClock/10);
  
  //PINSEL_ConfigPin(1,5,0); //CAS
  //GPIO_SetDir(1,1<<5,1);
  //GPIO_SetValue(1,1<<5);
  
  
  console_sendString((uint8_t*)"FatFS Demo\r\n");
  
  TurnOnSDCARD_Module(ON_MODE);
  
  res = f_mount(0, &lFatfs[0]);
  if (res != FR_OK) {
    sprintf(buff, "Failed to mount 0: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  console_sendString((uint8_t*)"Opening '/'\r\n");
  res = f_opendir(&dir, "/");
  if (res) {
    sprintf(buff, "Failed to open /: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  
  console_sendString((uint8_t*)"Listing content of '/'\r\n");
  for(;;) {
    res = f_readdir(&dir, &Finfo);
    if ((res != FR_OK) || !Finfo.fname[0]) break;
    
    console_sendString((uint8_t*)&(Finfo.fname[0]));
    console_sendString((uint8_t*)"\r\n");
  }
  
  console_sendString((uint8_t*)"\r\nOpening a file\r\n");
  res = f_open (&file, "a.bin", FA_READ);
  if (res) {
    sprintf(buff, "Failed to open a.bin: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  /***/
  
  //volatile uint8_t *byte_wr_ptr;
  //byte_wr_ptr = (uint8_t *)SDRAM_BASE;
  byte_wr_ptr = (void *)SDRAM_BASE;
 
  
 /***/
  
  
  
  
  console_sendString((uint8_t*)"Reading content of the file\r\n");
  res =  f_read (&file, byte_wr_ptr, file.fsize  , &numRead);			/* Read data from a file */
  if (res || numRead <= 0) {
    sprintf(buff, "Failed to read a.bin: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  buff[numRead] = '\0';
  console_sendString((uint8_t*)buff);
  
  console_sendString((uint8_t*)"\r\nClosing file\r\n");
  res =  f_close (&file);
  if (res) {
    sprintf(buff, "Failed to close ip.txt: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }

  TurnOnSDCARD_Module(OFF_MODE);
  
  tt = program_application_using_bootloader((unsigned char *)byte_wr_ptr,numRead);
  
  return(tt);

  
/*  
  console_sendString((uint8_t*)"\r\nOpening a file for writing\r\n");
  res = f_open (&file, "new.txt", (FA_CREATE_ALWAYS|FA_READ|FA_WRITE));
  if (res) {
    sprintf(buff, "Failed to open new.txt: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  console_sendString((uint8_t*)"Writing  to the file\r\n");
  res = f_write (&file, "Data written to file\r\n", 22, &numWritten);
  if (res) {
    sprintf(buff, "Failed to write to new.txt: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;

  }
  
  console_sendString((uint8_t*)"Closing file\r\n");
  res =  f_close (&file);
  if (res) {
    sprintf(buff, "Failed to close new.txt: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  */
  
  
}
