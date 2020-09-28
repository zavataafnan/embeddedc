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
#ifndef __SDRAM_H
#define __SDRAM_H

//#define SAMSUNG_RAM
#define ISSI_RAM
//#define RAM_TYPE SAMSUNG_RAM
//#define RAM_TYPE  ISSI_RAM
/*
 * These timing parameters are based on the EMC clock
 * there is no way of ensuring what the EMC clock frequency is
 * without severely bloating the code
 * ENSURE THAT THE EMC clock is one of these values
 */

#define tCLK_ns ((double)EMCClock / 1000000000.0 )                   // CCLK period in ns       
#define NS_2_CLKS(ns) ( (uint32_t)( (double)(ns) * tCLK_ns ) + 1 )   // convert ns to CCLKs 


#define SDRAM_SPEED_48 0
#define SDRAM_SPEED_50 1
#define SDRAM_SPEED_60 2
#define SDRAM_SPEED_72 3

#define SDRAM_TRP  20
#define SDRAM_TRAS  45
#define SDRAM_TXSR  tCLK_ns
#define SDRAM_TAPR  tCLK_ns
#define SDRAM_TDAL  20+tCLK_ns*2
#define SDRAM_TWR   tCLK_ns * 2
#define SDRAM_TRC   66
#define SDRAM_TRFC  66
#define SDRAM_TXSR   tCLK_ns
#define SDRAM_TRRD  15
#define SDRAM_TMRD  tCLK_ns * 2
#define SDRAM_REFRESH  7813


#define SDRAM_SPEED SDRAM_SPEED_60

#define SDRAM_CONFIG_32BIT

//#define SDRAM256
#define SDRAM64

#ifdef SDRAM64
#define SDRAM_SIZE               0x800000
#define divider1                 0x20000
#define divider2                 0x8000
#else
#define SDRAM_SIZE               0x2000000
#define divider1                 0x40000
#define divider2                 0x10000
#endif

#define SDRAM_BASE               0xA0000000 /*CS0*/

extern uint32_t sdram_init(void);
extern uint32_t sdram_test( void );
extern unsigned char sdram_test_all(void);


#endif /* end __SDRAM_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
