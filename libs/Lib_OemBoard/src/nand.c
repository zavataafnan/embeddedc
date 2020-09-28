/*****************************************************************************
*
*   Copyright(C) 2011, Embedded Artists AB
*   All rights reserved.
*
******************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* Embedded Artists AB assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. Embedded Artists AB
* reserves the right to make changes in the software without
* notification. Embedded Artists AB also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
*****************************************************************************/


/******************************************************************************
* Includes
*****************************************************************************/

#include "lpc_types.h"
#include "lpc177x_8x_timer.h"
#include "nand.h"
#include "RelayController.h"
#include "string.h"
#include "stdlib.h"

/******************************************************************************
* Defines and typedefs
*****************************************************************************/

#define K9F2G_CLE   ((volatile uint8_t *)0x90100000)
#define K9F2G_ALE   ((volatile uint8_t *)0x90080000)
#define K9F2G_DATA  ((volatile uint8_t *)0x90000000)

//#define K9FXX_WAIT()

#define K9FXX_READ_1            0x00                
#define K9FXX_READ_2            0x30                

//#define K9FXX_SET_ADDR_A        0x00                
//#define K9FXX_SET_ADDR_B        0x01                
//#define K9FXX_SET_ADDR_C        0x50                
#define K9FXX_READ_ID           0x90                
//#define K9FXX_RESET             0xff                
#define K9FXX_BLOCK_PROGRAM_1   0x80                
#define K9FXX_BLOCK_PROGRAM_2   0x10                
#define K9FXX_BLOCK_ERASE_1     0x60                
#define K9FXX_BLOCK_ERASE_2     0xd0                
#define K9FXX_READ_STATUS       0x70                
//#define K9FXX_BUSY              (1 << 6)            
//#define K9FXX_OK                (1 << 0)   

#define ID_MARKER_CODE (0xEC)
#define ID_SAMSUNG     (0xDA)

//#define ID_PAGE_SZ_1KB (0x00)
//#define ID_PAGE_SZ_2KB (0x01)
//#define ID_PAGE_SZ_4KB (0x02)
//#define ID_PAGE_SZ_8KB (0x03)

//#define ID_BLOCK_SZ_64KB  (0x00)
//#define ID_BLOCK_SZ_128KB (0x01)
//#define ID_BLOCK_SZ_256KB (0x02)
//#define ID_BLOCK_SZ_512KB (0x03)

//#define ID_PAGE_SZ_1KB (0x00)
//#define ID_PAGE_SZ_2KB (0x01)
//#define ID_PAGE_SZ_4KB (0x02)
//#define ID_PAGE_SZ_8KB (0x03)

//#define ID_REDUND_SZ_8  (0x00)
//#define ID_REDUND_SZ_16 (0x01)



/* This macro could be changed to check the ready pin */
#define WAIT_READY() (TIM_Waitus(50))


/******************************************************************************
* External global variables
*****************************************************************************/
// nand remap table
extern uint16_t nand_address_remap[NAND_NUM_BLOCKS];
/******************************************************************************
* Local variables
*****************************************************************************/

static uint32_t pageSize   = 0;
static uint32_t blockSize  = 0;
static uint32_t reduntSize = 0;


/******************************************************************************
* Local Functions
*****************************************************************************/

static void pinConfigNAND(void)
{
	
  //LPC_IOCON->P3_0 |= 1; /* D0 @ P3.0 */
  //LPC_IOCON->P3_1 |= 1; /* D1 @ P3.1 */
  //LPC_IOCON->P3_2 |= 1; /* D2 @ P3.2 */
  //LPC_IOCON->P3_3 |= 1; /* D3 @ P3.3 */

  //LPC_IOCON->P3_4 |= 1; /* D4 @ P3.4 */
  //LPC_IOCON->P3_5 |= 1; /* D5 @ P3.5 */
  //LPC_IOCON->P3_6 |= 1; /* D6 @ P3.6 */
  //LPC_IOCON->P3_7 |= 1; /* D7 @ P3.7 */

  //LPC_IOCON->P3_8 |= 1; /* D8 @ P3.8 */
  //LPC_IOCON->P3_9 |= 1; /* D9 @ P3.9 */
  //LPC_IOCON->P3_10 |= 1; /* D10 @ P3.10 */
  //LPC_IOCON->P3_11 |= 1; /* D11 @ P3.11 */

  //LPC_IOCON->P3_12 |= 1; /* D12 @ P3.12 */
  //LPC_IOCON->P3_13 |= 1; /* D13 @ P3.13 */
  //LPC_IOCON->P3_14 |= 1; /* D14 @ P3.14 */
  //LPC_IOCON->P3_15 |= 1; /* D15 @ P3.15 */

  //LPC_IOCON->P3_16 |= 1; /* D16 @ P3.16 */
  //LPC_IOCON->P3_17 |= 1; /* D17 @ P3.17 */
//LPC_IOCON->P3_18 |= 1; /* D18 @ P3.18 */
  //LPC_IOCON->P3_19 |= 1; /* D19 @ P3.19 */

  //LPC_IOCON->P3_20 |= 1; /* D20 @ P3.20 */
  //LPC_IOCON->P3_21 |= 1; /* D21 @ P3.21 */
  //LPC_IOCON->P3_22 |= 1; /* D22 @ P3.22 */
  //LPC_IOCON->P3_23 |= 1; /* D23 @ P3.23 */

  //LPC_IOCON->P3_24 |= 1; /* D24 @ P3.24 */
  //LPC_IOCON->P3_25 |= 1; /* D25 @ P3.25 */
  //LPC_IOCON->P3_26 |= 1; /* D26 @ P3.26 */
  //LPC_IOCON->P3_27 |= 1; /* D27 @ P3.27 */

  //LPC_IOCON->P3_28 |= 1; /* D28 @ P3.28 */
  //LPC_IOCON->P3_29 |= 1; /* D29 @ P3.29 */
  //LPC_IOCON->P3_30 |= 1; /* D30 @ P3.30 */
  //LPC_IOCON->P3_31 |= 1; /* D31 @ P3.31 */

  //LPC_IOCON->P4_0 |= 1; /* A0 @ P4.0 */
  //LPC_IOCON->P4_1 |= 1; /* A1 @ P4.1 */
  //LPC_IOCON->P4_2 |= 1; /* A2 @ P4.2 */
  //LPC_IOCON->P4_3 |= 1; /* A3 @ P4.3 */

  //LPC_IOCON->P4_4 |= 1; /* A4 @ P4.4 */
  //LPC_IOCON->P4_5 |= 1; /* A5 @ P4.5 */
  //LPC_IOCON->P4_6 |= 1; /* A6 @ P4.6 */
  //LPC_IOCON->P4_7 |= 1; /* A7 @ P4.7 */

  //LPC_IOCON->P4_8 |= 1; /* A8 @ P4.8 */
  //LPC_IOCON->P4_9 |= 1; /* A9 @ P4.9 */
  //LPC_IOCON->P4_10 |= 1; /* A10 @ P4.10 */
  //LPC_IOCON->P4_11 |= 1; /* A11 @ P4.11 */

  //LPC_IOCON->P4_12 |= 1; /* A12 @ P4.12 */
  //LPC_IOCON->P4_13 |= 1; /* A13 @ P4.13 */
  //LPC_IOCON->P4_14 |= 1; /* A14 @ P4.14 */
  //LPC_IOCON->P4_15 |= 1; /* A15 @ P4.15 */

  //LPC_IOCON->P4_16 |= 1; /* A16 @ P4.16 */
  //LPC_IOCON->P4_17 |= 1; /* A17 @ P4.17 */
  //LPC_IOCON->P4_18 |= 1; /* A18 @ P4.18 */
  LPC_IOCON->P4_19 |= 1; /* A19 @ P4.19 */

  LPC_IOCON->P4_20 |= 1; /* A20 @ P4.20 */
  //LPC_IOCON->P4_21 |= 1; /* A21 @ P4.21 */
  //LPC_IOCON->P4_22 |= 1; /* A22 @ P4.22 */
  //LPC_IOCON->P4_23 |= 1; /* A23 @ P4.23 */

  LPC_IOCON->P4_24 |= 1; /* OEN @ P4.24 */
  LPC_IOCON->P4_25 |= 1; /* WEN @ P4.25 */
  LPC_IOCON->P4_26 |= 1; /* BLSN[0] @ P4.26 */
  //LPC_IOCON->P4_27 |= 1; /* BLSN[1] @ P4.27 */

  //LPC_IOCON->P4_28 |= 1; /* BLSN[2] @ P4.28 */
  //LPC_IOCON->P4_29 |= 1; /* BLSN[3] @ P4.29 */
  //LPC_IOCON->P4_30 |= 1; /* CSN[0] @ P4.30 */
  LPC_IOCON->P4_31 |= 1; /* CSN[1] @ P4.31 */

  //LPC_IOCON->P2_14 |= 1; /* CSN[2] @ P2.14 */
  //LPC_IOCON->P2_15 |= 1; /* CSN[3] @ P2.15 */
}


static uint32_t nandReadId(void)
{
  uint8_t a, b, c, d;
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  *pCLE = K9FXX_READ_ID;
  *pALE = 0;
  
  a = *pData;
  b = *pData;
  c = *pData;
  d = *pData;
  
  
  return (a << 24) | (b << 16) | (c << 8) | d;
}

static uint8_t nandStatus(void)
{
  uint8_t status = 0;
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  *pCLE = K9FXX_READ_STATUS;
  *pALE = 0;
  
  status = *pData;
  
  /* remove bits not used */
  return (status & 0xC1);  
}

static void nandWaitReady(void) 
{
  while( !(nandStatus() & (1<<6)) );
}

/******************************************************************************
* Public Functions
*****************************************************************************/

static char buff[2112];

char nand_test(void)
{
  int i = 0;
  int blockNum = 100;
	int tt;
  
  //console_sendString((uint8_t*)"Checking blocks\r\n");      
  for (i = 0; i < NAND_NUM_BLOCKS; i++) {
    if (nand_isBlockValid(i)!=0) {
      sprintf(buff, "Block %d is BAD\r\n", i);
      console_sendString((uint8_t*)buff);      
    }
  }
  console_sendString((uint8_t*)"DONE\r\n\r\n"); 
  tt = nand_getPageSize();
  for (i = 0; i < nand_getPageSize(); i++) {
    buff[i] = (char)(i+1);
  }

  console_sendString((uint8_t*)"Erasing...\r\n");        
  
  if (nand_eraseBlock(blockNum)!=0) {
    console_sendString((uint8_t*)"Failed to erase block\r\n");        
  }

  console_sendString((uint8_t*)"Writing...\r\n");          
  if (nand_writePage(blockNum, 0, (uint8_t*) buff)!=0) {
    console_sendString((uint8_t*)"Failed to write to NAND\r\n");        
    return 1;
  }

  memset(buff, 0, 2112);

  console_sendString((uint8_t*)"Reading...\r\n");          
  if (nand_readPage(blockNum, 0, (uint8_t*) buff)!=0) {
    console_sendString((uint8_t*)"Failed to read from NAND\r\n");        
    return 1;
  }

  console_sendString((uint8_t*)"Verifing...\r\n");        
  for (i = 0; i < nand_getPageSize(); i++) {
    if (buff[i] != (char)(i+1)) {
      console_sendString((uint8_t*)"Verify failed\r\n");        
      return 1;
    }
  }
  console_sendString((uint8_t*)"Done\r\n");        


  return 0;
}

/*
char nand_test(void)
{
  
  
  char flag = 1;  
  int i = 0;
  int bs = blockSize/pageSize;
  int blockNum = 0;
  int pageNum = 0;
	
	console_sendString((uint8_t*)"Checking blocks\r\n"); 
  
  for (i = 0; i < NAND_NUM_BLOCKS; i++) {
    if (!nand_isBlockValid(i)) {
      sprintf(buff, "Block %d is BAD\r\n", i);
      console_sendString((uint8_t*)buff); 
      
    }
  }
  console_sendString((uint8_t*)"DONE\r\n\r\n"); 
  
  
  for ( blockNum = 0 ; blockNum < NAND_NUM_BLOCKS ; blockNum++)
  {
    
    if (!nand_eraseBlock(blockNum)) {
      console_sendString((uint8_t*)"Failed to erase block\r\n");  
      
      break;
    }
    
    for ( pageNum = 0 ; pageNum < bs ; pageNum++)
    {
      
      for (i = 0; i < nand_getPageSize(); i++) {
        buff[i] = (char)(i+1);
      }
      
      
      
      
      
      
      if (!nand_writePage(blockNum, pageNum, (uint8_t*) buff)) {
        console_sendString((uint8_t*)"Failed to write to NAND\r\n");        
        return 1;
      }
      
      memset(buff, 0, 2112);
      
      //console_sendString((uint8_t*)"Reading...\r\n");          
      if (!nand_readPage(blockNum, pageNum, (uint8_t*) buff)) {
        console_sendString((uint8_t*)"Failed to read from NAND\r\n");        
        return 1;
      }
      
      //console_sendString((uint8_t*)"Verifing...\r\n");        
      for (i = 0; i < nand_getPageSize(); i++) {
        if (buff[i] != (char)(i+1)) {
          console_sendString((uint8_t*)"Verify failed\r\n");        
          flag = 0;
          //break;
        }
      }
    }
  }
  
  if(flag == 1)
    console_sendString((uint8_t*)"Done\r\n");   
  else
    console_sendString((uint8_t*)"Unsuccessful\r\n");  
  
  return flag;
  
}
*/

/******************************************************************************
*
* Description:
*    Initialize the NAND Flash
*
* Returns:
*    0: if initialization successful
*    1: if unknown chip
*
*****************************************************************************/
uint32_t nand_init (void)
{
  uint32_t nandId = 0;
	uint16_t i=0,j=0;
	uint8_t shifter=0;
  TIM_TIMERCFG_Type timerCfg;
  
  LPC_SC->PCONP     |= 0x00000800;
  LPC_EMC->Control   = 0x00000001;
  LPC_EMC->Config    = 0x00000000;

  pinConfigNAND();
  
  NFR_NAND_Module();
  TrunOnWriteProtectionNAND_Module(OFF_MODE);
  
  
  TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);
  
  LPC_EMC->StaticConfig1   = 0x00000080;
  
  LPC_EMC->StaticWaitWen1  = 0x00000002; 
  LPC_EMC->StaticWaitOen1  = 0x00000002; 
  LPC_EMC->StaticWaitRd1   = 0x00000008; 
  LPC_EMC->StaticWaitPage1 = 0x0000001f; 
  LPC_EMC->StaticWaitWr1   = 0x00000008; 
  LPC_EMC->StaticWaitTurn1 = 0x0000000f;
  
  nandId = nandReadId();
  
  if ((nandId & 0xffff0000) != 
      (((uint32_t)(ID_MARKER_CODE) << 24) | ID_SAMSUNG << 16)) {
        /* unknown NAND chip */
        return(1);
      }
  
  pageSize   = 1024 * (1 << (nandId & 0x03));  
  blockSize  = 64*1024 * (1 << ((nandId>>4) & 0x03));
  reduntSize = 8 * (1 << ((nandId >> 1) & 0x1));
	
	//OK
  return(0);
}

/******************************************************************************
*
* Description:
*    Get the page size of the NAND flash
*
* Returns:
*    page size in bytes
*
*****************************************************************************/
uint32_t nand_getPageSize(void)
{
  return pageSize;
}

/******************************************************************************
*
* Description:
*    Get the block size of the NAND flash
*
* Returns:
*    block size in bytes
*
*****************************************************************************/
uint32_t nand_getBlockSize(void)
{
  return blockSize;
}

/******************************************************************************
*
* Description:
*    Get the redundant (spare) size per page
*
* Returns:
*    redundant/spare size in bytes
*
*****************************************************************************/
uint32_t nand_getRedundantSize(void)
{
  return reduntSize * (pageSize/512);
}

/******************************************************************************
*
* Description:
*    Check if a block is valid
*
* Returns:
*    0: if the block is valid
*    2: if input block number is higher than total nand flash blocks
*    5: if invalid
*
*****************************************************************************/
uint32_t nand_isBlockValid(uint32_t block)
{
  uint32_t addr = 0;
  uint32_t page = 0;
  
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  if (block >= NAND_NUM_BLOCKS) {
    return(2);
  }
  
  addr = block * (blockSize/pageSize);
  
  /* 
  * Check page 0 and page 1 in each block. If the first byte
  * in the spare area (of either page 0 or page 1) is != 0xFF 
  * the block is invalid.
  */
  
  nandWaitReady();
  
  for (page = 0; page < 2; page++) {
    addr += page;
    
    *pCLE = K9FXX_READ_1;
    *pALE = (uint8_t) (pageSize & 0x00FF);
    *pALE = (uint8_t)((pageSize & 0xFF00) >> 8);
    *pALE = (uint8_t)((addr & 0x00FF));
    *pALE = (uint8_t)((addr & 0xFF00) >> 8);
		//2gbit
    *pALE = (uint8_t)((addr & 0xFF0000) >> 16);
    
		*pCLE = K9FXX_READ_2;
    
    WAIT_READY();
    
    if (*pData != 0xFF) {
      return(1);
    }
    
  }
  
  return(0);
}


/******************************************************************************
*
* Description:
*    Read a page from the NAND memory
*
* Params:
*    block - block number to read from
*    page  - page withín block to read from
*    pageBuf - data is copied to this buffer. The size must be at least 
*              pageSize.
*
* Returns:
*    0: if read successful
*    1: if not succesful
*    2: if input block number is higher than total nand flash blocks
*    3: if page number is higher than total pages in each block
*
*****************************************************************************/
uint32_t nand_readPage(uint32_t block, uint32_t page, uint8_t* pageBuf)
{
  uint32_t i = 0;
  uint32_t addr = 0;
  
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  if (block >= NAND_NUM_BLOCKS) {
    return(2);
  }
  
  if (page >= blockSize/pageSize) {
    return(3);
  }
  
  addr = block * (blockSize/pageSize) + page;
  
  /*
  * Always reading from start of a page address.
  * This means that the column address is always 0.
  */
  
  *pCLE = K9FXX_READ_1;
  *pALE = 0;
  *pALE = 0;
	*pALE = (uint8_t)((addr & 0x00FF));
  *pALE = (uint8_t)((addr & 0xFF00) >> 8);
  //2gbit
  *pALE = (uint8_t)((addr & 0xFF0000) >> 16);
	
  *pCLE = K9FXX_READ_2;
  
  WAIT_READY(); 
  
  
  for (i = 0; i < pageSize; i++) {
    *pageBuf++ = *pData;  
  }
  
  
  return(0);
}

/******************************************************************************
*
* Description:
*    Write a page of data to the NAND memory
*
* Params:
*    block - block number to write to
*    page  - page within block to write to
*    pageBuf - data is copied from this buffer. The size must be at least 
*              pageSize.
*
* Returns:
*    0: if read successful
*    1: if not succesful
*    2: if input block number is higher than total nand flash blocks
*    3: if page number is higher than total pages in each block
*
*****************************************************************************/
uint32_t nand_writePage(uint32_t block, uint32_t page, uint8_t* pageBuf)
{
  uint32_t i = 0;
  uint32_t addr = 0;
  
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  if (block >= NAND_NUM_BLOCKS) {
    return(2);
  }
  
  if (page >= blockSize/pageSize) {
    return(3);
  }
  
  addr = block * (blockSize/pageSize) + page;
  
  /*
  * Always writing to start of a page address.
  * This means that the column address is always 0.
  */ 
  
  *pCLE = K9FXX_BLOCK_PROGRAM_1;
  *pALE = 0; 
  *pALE = 0; 
  *pALE = (uint8_t)((addr & 0x00FF));
  *pALE = (uint8_t)((addr & 0xFF00) >> 8);
	//2gbit
  *pALE = (uint8_t)((addr & 0xFF0000) >> 16);
  
  
  for (i = 0; i < pageSize; i++) {
    *pData = *pageBuf++;
  }
  
  *pCLE = K9FXX_BLOCK_PROGRAM_2;
  
	//2gbit
  TIM_Waitus(750);  
  nandWaitReady();
  
  return ((nandStatus() & 0x01)); 
}

/******************************************************************************
*
* Description:
*    Erase a block
*
* Params:
*    block - block number to erase
*
* Returns:
*    0: if erase successful
*    1: if erase not succesful
*    2: if input block number is higher than total nand flash blocks
*
*****************************************************************************/
uint32_t nand_eraseBlock(uint32_t block)
{
  uint32_t addr = 0;
  
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  
  if (block >= NAND_NUM_BLOCKS) {
    return(2);
  }
  
  addr = block * (blockSize/pageSize);
  
  *pCLE = K9FXX_BLOCK_ERASE_1;
  *pALE = (uint8_t)(addr & 0x00FF);
  *pALE = (uint8_t)((addr & 0xFF00) >> 8);
		//2gbit
  *pALE = (uint8_t)((addr & 0xFF0000) >> 16);
	
  *pCLE = K9FXX_BLOCK_ERASE_2;
  
	//2gbit
  TIM_Waitus(750);
  nandWaitReady();
  
  return ((nandStatus() & 0x01)); 
}

/******************************************************************************
*
* Description:
*    Read a complete block from the NAND memory
*
* Params:
*    block - block number to read from
*    pageBuf - data is copied to this buffer. The size must be at least 
*              blocksize.
*
* Returns:
*    0: if read successful
*    1: if not succesful
*    2: if input block number is higher than total nand flash blocks
*    3: if page number is higher than total pages in each block
*    4: if checksum error during read from block memory
*
*****************************************************************************/
uint32_t nand_readBlock(uint32_t block, uint8_t* Buffer)
{

  uint8_t page_counter;
	uint8_t stat;
	
	for(page_counter=0;page_counter<64;page_counter++)
	{
	  stat = nand_readPage(block, page_counter, Buffer);
		if(stat != 0)
			return(stat);
		Buffer = Buffer + pageSize;
	}

	return(0);
}

/******************************************************************************
*
* Description:
*    Write a complete block to the NAND memory
*
* Params:
*    block - block number to write to
*    pageBuf - data is copied from this buffer. The size must be at least 
*              blocksize.
*
* Returns:
*    0: if write successful
*    1: if not succesful
*    2: if input block number is higher than total nand flash blocks
*    3: if page number is higher than total pages in each block
*****************************************************************************/
uint32_t nand_writeBlock(uint32_t block, uint8_t* Buffer)
{

  uint8_t page_counter;
	uint8_t stat;
	
	for(page_counter=0;page_counter<64;page_counter++)
	{
	  stat = nand_writePage(block, page_counter, Buffer);
		if(stat != 0)
			return(stat);
		Buffer = Buffer + pageSize;
	}

	return(0);
}

/******************************************************************************
*
* Description:
*    Write flash partition
*
* Params:
*    block - block number to write to
*    section  - section A or B (0 and 1) or higher depending on SECTIONS in .h file
*    buffer - data is copied from this buffer. The size must be at least 
*              blocksize/sections.
*
* Returns:
*    0: if successful
*    1: if not succesful
*    2: if input block number is higher than total nand flash blocks
*    3: if page number is higher than total pages in each block
*    4: if checksum error during read from block memory
*    5: if block invalid
*    6: invalid section number
*****************************************************************************/
uint8_t nand_writepartition(uint32_t block, uint8_t section, uint8_t* buffer)
{
  
	uint32_t stat;
	uint8_t * blockbuffer_ptr;
	uint8_t * comparebuffer_ptr;
	
	//if section too high
	if(section>SECTIONS-1)
	{
	 return(6);
	}
	
	stat = nand_isBlockValid(block);
	
	//if error
	if(stat != 0)
	{
	  return(stat);
	}
  
	//no error, copy section data to buffer
	//create buffer for complete block
	blockbuffer_ptr = (uint8_t *)malloc(blockSize);
	
	//copy all block data to blockbuffer
	stat = nand_readBlock(block, blockbuffer_ptr);
	
	//if error
	if(stat != 0)
	{
		free(blockbuffer_ptr);
	  return(stat);
	}

	//copy new section into blockbuffer
	if(section==0)
	{
	   memcpy(blockbuffer_ptr,buffer,blockSize/SECTIONS);
	}
	else if(section==1)
	{
	 	 memcpy(blockbuffer_ptr + blockSize/SECTIONS ,buffer,blockSize/SECTIONS);
	}
	else if(section==2)
	{
	 	 memcpy(blockbuffer_ptr + 2 * (blockSize/SECTIONS),buffer,blockSize/SECTIONS);
	}
	else if(section==3)
	{
	 	 memcpy(blockbuffer_ptr + 3 * (blockSize/SECTIONS),buffer,blockSize/SECTIONS);
	}
	
	//erase block for writing
	stat = nand_eraseBlock(block);
	//if error
	if(stat != 0)
	{
		free(blockbuffer_ptr);
	  return(stat);
	}
	
	//write complete blockbuffer data  into block
	stat = nand_writeBlock(block, blockbuffer_ptr);
	
	//re_read buffer to see if written correctly
	comparebuffer_ptr = (uint8_t *)malloc(blockSize);
	
	//copy all block data to comparebuffer
	stat = nand_readBlock(block, comparebuffer_ptr);
	
	//if error
	if(stat != 0)
	{
	  free(comparebuffer_ptr);
	  free(blockbuffer_ptr);
	  return(stat);
	}
	
	//compare buffers
	if(memcmp(blockbuffer_ptr,comparebuffer_ptr,blockSize)!=0)
	{
	 free(comparebuffer_ptr);
	 free(blockbuffer_ptr);
	 //block is invalid
	 return(5);
	}
	
	free(comparebuffer_ptr);
	free(blockbuffer_ptr);
	return(1);
	
}

uint32_t nand_writeAFile(uint32_t file_location, uint8_t* pageBuf, uint32_t buf_size)
{
  uint32_t i = 0;
  uint32_t addr = 0;
  
	uint8_t page_num = buf_size / pageSize;
	uint32_t block_number = file_location / 4;
	uint16_t page_offset = (file_location % 4)* 0x10;
	
  volatile uint8_t *pCLE;
  volatile uint8_t *pALE;
  volatile uint8_t *pData;
  
  
  pCLE  = K9F2G_CLE;
  pALE  = K9F2G_ALE;
  pData = K9F2G_DATA;
  
  if (file_location >= NAND_NUM_BLOCKS*2) {
    return FALSE;
  }

	//uint32_t nand_writePage(uint32_t block, uint32_t page, uint8_t* pageBuf)
	
	for (i = 0; i < page_num ; i++)
	{
		nand_writePage(block_number, page_offset + i, pageBuf + i * (pageSize));
	}
  
  
  
  return 1;
}


unsigned char Catch_File_From_SDCARD(void)
{
	DIR dir;
  BYTE res;
  FIL file;
  UINT numRead = 0;
  UINT numWritten = 0;
  TIM_TIMERCFG_Type timerCfg;
  FATFS lFatfs[1];
  FILINFO Finfo;
	uint8_t * byte_wr_ptr;
  uint32_t * u32_wr_ptr;
	uint16_t lic_num = 0;
  unsigned char tt;
  
  uint8_t buff[512];
  // initialize timer
  TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);

  SysTick_Config(SystemCoreClock/10);
  
  
  TurnOnSDCARD_Module(ON_MODE);
  
  res = f_mount(0, &lFatfs[0]);
  if (res != FR_OK) {
    return 0;
  }

  res = f_opendir(&dir, "/");
  if (res) {
    return 0;
  }
  

  for(;;) {
    res = f_readdir(&dir, &Finfo);
    if ((res != FR_OK) || !Finfo.fname[0]) break;
    
		//if containing the license : copy to the address:
		  res = f_open (&file, "type_a.bin", FA_READ);
			if (res) {
				return 0;
			}
			
			byte_wr_ptr = (uint8_t *)malloc(file.fsize);
			
		  res =	f_read (&file, byte_wr_ptr, file.fsize	, &numRead);			// Read data from a file 
			if (res || numRead <= 0) {
				return 0;
			}
			
			
			//do something :
			//uint32_t nand_writeAFile(uint32_t file_location, uint8_t* pageBuf, uint32_t buf_size)
			nand_writeAFile(lic_num,byte_wr_ptr,file.fsize);
			
			free(byte_wr_ptr);
	
  }
  

  
  
  
  //volatile uint8_t *byte_wr_ptr;
  byte_wr_ptr = (uint8_t *)(0x10000000);
  //void * byte_wr_ptr = (void *)SDRAM_BASE;
  //uint8_t * byte_wr_ptr = (uint8_t *)malloc(file.fsize);
  u32_wr_ptr = (uint32_t *)(0x10000000);

  console_sendString((uint8_t*)"Reading content of the file\r\n");
  res =  f_read (&file, byte_wr_ptr, file.fsize  , &numRead);			// Read data from a file 
  if (res || numRead <= 0) {
    sprintf(buff, "Failed to read type_a.bin: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }
  
  //buff[numRead] = '\0';
  //console_sendString((uint8_t*)buff);
  
  console_sendString((uint8_t*)"\r\nClosing file\r\n");
  res =  f_close (&file);
  if (res) {
    sprintf(buff, "Failed to close ip.txt: %d \r\n", res);
    console_sendString((uint8_t*)buff);
    return 0;
  }

  TurnOnSDCARD_Module(OFF_MODE);
  

  return(tt);

  
}

/******************************************************************************
*
* Description:
*    Copies all ECU data from SDRAM and writes them into nand flash
*
* Returns:
*    0: if successful
*    1: SDCARD mount failure (multiple codes returned)
*    2: failed to open ECU folder
*    3: if page number is higher than total pages in each block
*    4: if checksum error during read from block memory
*    5: if block invalid
*    6: invalid section number
*****************************************************************************/
uint8_t copy_ecus_from_sd_to_nand(void)
{
  
  
  DIR dir;
  BYTE res;
  FIL file;
  UINT numRead = 0;
  UINT numWritten = 0;
	FATFS lFatfs[1];
	FILINFO Finfo;

  TIM_TIMERCFG_Type timerCfg;
	void * byte_wr_ptr;
  unsigned char tt;
  
  uint8_t buff[512];
  // initialize timer
  TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);
  
  //console_init();
  
  SysTick_Config(SystemCoreClock/10);
  
  //PINSEL_ConfigPin(1,5,0); //CAS
  //GPIO_SetDir(1,1<<5,1);
  //GPIO_SetValue(1,1<<5);
  
  
  //console_sendString((uint8_t*)"FatFS Demo\r\n");
  
  TurnOnSDCARD_Module(ON_MODE);
  
  res = f_mount(0, &lFatfs[0]);
  if (res != FR_OK) {
    //sprintf(buff, "Failed to mount 0: %d \r\n", res);
    //console_sendString((uint8_t*)buff);
    return(1);
  }
  
  //console_sendString((uint8_t*)"Opening '/'\r\n");
  res = f_opendir(&dir, "/ECU");
  if (res) {
    //sprintf(buff, "Failed to open /: %d \r\n", res);
    //console_sendString((uint8_t*)buff);
		//failed to open ECU folder
    return(2);
  }
  
  
  //console_sendString((uint8_t*)"Listing content of '/'\r\n");
  for(;;) 
	{
    res = f_readdir(&dir, &Finfo);
    if ((res != FR_OK) || !Finfo.fname[0])
		{			
			break;
		}
    
    //console_sendString((uint8_t*)&(Finfo.fname[0]));
    //console_sendString((uint8_t*)"\r\n");
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
  byte_wr_ptr = (uint8_t *)malloc(file.fsize);
 
  
 /***/
  
  
  
  
  //console_sendString((uint8_t*)"Reading content of the file\r\n");
  res =  f_read (&file, byte_wr_ptr, file.fsize  , &numRead);			/* Read data from a file */
  if (res || numRead <= 0) {
    //sprintf(buff, "Failed to read a.bin: %d \r\n", res);
    //console_sendString((uint8_t*)buff);
    return 0;
  }
  
  //buff[numRead] = '\0';
  //console_sendString((uint8_t*)buff);
  
  //console_sendString((uint8_t*)"\r\nClosing file\r\n");
  res =  f_close (&file);
  if (res) {
    //sprintf(buff, "Failed to close ip.txt: %d \r\n", res);
   // console_sendString((uint8_t*)buff);
    return 0;
  }

  TurnOnSDCARD_Module(OFF_MODE);
  
  tt = program_application_using_bootloader((unsigned char *)byte_wr_ptr,numRead);
  
  free(byte_wr_ptr);

  return(tt);

}

