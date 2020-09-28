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
#ifndef __NAND_H
#define __NAND_H
#include "lpc_types.h"

//1024 for 1gbit device
//2048 for 2gbit device
#define NAND_NUM_BLOCKS (2048)
#define SECTIONS         2     //number of sections in each block
#define NAND_BLOCK_SIZE 0x20000
#define PARTITION_SIZE NAND_BLOCK_SIZE/PARTITIONS

typedef struct _ECU_LICENSES__
{
  uint16_t block_number;
  uint8_t block_section;
  char verison[6];
	uint16_t file_size;
  uint8_t enabled;
	uint8_t license_ecus[32];
	
  
} ECU_LICENSES;


extern uint32_t nand_init (void);
extern uint32_t nand_getPageSize(void);
extern uint32_t nand_getBlockSize(void);
extern uint32_t nand_getRedundantSize(void);
extern uint32_t nand_isBlockValid(uint32_t blockNum);
uint32_t nand_readPage(uint32_t block, uint32_t page, uint8_t* pageBuf);
uint32_t nand_writePage(uint32_t block, uint32_t page, uint8_t* pageBuf);
uint32_t nand_eraseBlock(uint32_t block);
uint32_t nand_writeAFile(uint32_t file_location, uint8_t* pageBuf, uint32_t buf_size);
uint8_t nand_writepartition(uint32_t block, uint8_t section, uint8_t* buffer);
uint8_t copy_ecus_from_sd_to_nand(void);
char nand_test(void);


#endif /* end __NAND_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
