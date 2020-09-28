#include "Uart_InterruptManager_1.h"
#include "stdint.h"
#include "stdlib.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc_types.h"
#include "globalDef.h"
#include "MbCRC.h"
#include "ErrHandling.h"
#include "CommProtocol.h"
#include "CRCGenerator.h"
#include "RelayController.h"
#include "lib_crc.h"

UART_RING_BUFFER_T rb1;



void UART1_IRQHandler(void)
{
  uint32_t intsrc, tmp, tmp1;
  
  
  intsrc = UART_GetIntId(UART_1);
  tmp = intsrc & UART_IIR_INTID_MASK;
  
  // Receive Line Status
  if (tmp == UART_IIR_INTID_RLS)
  {
    // Check line status
    tmp1 = UART_GetLineStatus(UART_1);
    // Mask out the Receive Ready and Transmit Holding empty status
    tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
    // If any error exist
    if (tmp1)
    {
      UART_IntErr1(tmp1);
    }
  }
  
  // Receive Data Available or Character time-out
  if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
  {
    uint8_t tmpc;
    uint32_t rLen;
    
    //while(1)
    {
      // Call UART read function in UART driver
      //rLen = UART_Receive((LPC_UART_TypeDef*)LPC_UART1, &tmpc, 1, NONE_BLOCKING, 500);
      //if (!rLen)
      //	break;
      
      UART_IntReceive1(UART_1);
    }
  }
}

/*
void UART1_IRQHandler(void)
{
uint32_t intsrc, tmp, tmp1;


intsrc = UART_GetLineStatus((LPC_UART_TypeDef*)LPC_UART1);
tmp = intsrc & UART_IIR_INTID_MASK;

// Receive Line Status
if (tmp == UART_IIR_INTID_RLS)
{
// Check line status
tmp1 = UART_GetLineStatus((LPC_UART_TypeDef*)LPC_UART1);
// Mask out the Receive Ready and Transmit Holding empty status
tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
// If any error exist
if (tmp1)
{
UART_IntErr1(tmp1);
		}
	}

// Receive Data Available or Character time-out
if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
{
UART_IntReceive1((LPC_UART_TypeDef*)LPC_UART1);
	}

}
*/

void Reset_UartRingBuffer1(void)
{
  //	int indxI;
  __BUF_RESET(rb1.rx_head);
  __BUF_RESET(rb1.rx_tail);
  __BUF_RESET(rb1.tx_head);
  __BUF_RESET(rb1.tx_tail);
  
  //	for (indxI = 0; indxI < UART_RING_BUFSIZE; indxI++)
  //	{
  //		rb1.rx[indxI] = 0xFF;
  //		rb1.tx[indxI] = 0xFF;
  //	}
}






/********************************************************************
* UART receive function (ring buffer used)
*********************************************************************/
void Write_ModeBusFrame(UART_ID_Type UartID, unsigned char* pyFrame, int nNumByte);

void UART_IntReceive1(UART_ID_Type UartID)
{
  
  int_char_little dump_int_to_char;
  uint8_t mbFunc = 0;
  //	uint16_t regAddr = 0xFFFF;
  uint8_t tmpc[5];
  uint32_t rLen;
  uint16_t num_of_byte = 0;
  uint16_t crc_check = 0;
  uint8_t frame_buf[30];
  uint8_t i,j;
  
  
  //while(1)
  {
    // Call UART read function in UART driver
    rLen = UART_Receive(UartID, tmpc, 1, NONE_BLOCKING, 500);
    if(rb1.rx_head == 1 && tmpc == 0x00)
    {
      Reset_UartRingBuffer1();
      return;
    }
    
    if (!rLen) // no more data v 
      return;
    
    // If data received
    
    /* Check if buffer is more space
    * If no more space, remaining character will be trimmed out
    */
    if (!__BUF_IS_FULL(rb1.rx_head,rb1.rx_tail))
    {
      rb1.rx[rb1.rx_head] = tmpc[0];
			 //Activate_LEDPin(LED_BUZZER,ON_MODE);
       //delay_ms(150);
			//Activate_LEDPin(LED_BUZZER,OFF_MODE);
      __BUF_INCR(rb1.rx_head);
      
      if(rb1.rx[0] == 0x80)
      {
        //num_of_byte = rb1.rx[1];
      }
      else
      {
        Reset_UartRingBuffer1();
      }
     
      
      if(isReadyFrame(rb1.rx,rb1.rx_head))
      {
        //check for the check sum !!!
        //AnalyzeFrame(rb1.rx,rb1.rx_head);

//      
        if(!isCheckSumCorrect(rb1.rx))
        {
          AnalyzeFrame(rb1.rx,rb1.rx_head);
        }
        else
        {
//          //check sum error! report!!!
          ErrorHandling(ERR1_CHECK_SUM_ERROR_FRAME);
        }
      
        Reset_UartRingBuffer1();
      }
    }
    else
    {
      Reset_UartRingBuffer1();
    }
    
    
    
  }
}


/*********************************************************************
* @brief    UART Line Status Error
* @param[in]  bLSErrType  UART Line Status Error Type
* @return    None
**********************************************************************/
void UART_IntErr1(uint8_t bLSErrType)
{
  /*
  if (bLSErrType & UART_LSR_PE)
  {
  UART_Send(LPC_UART0, p_err_menu, sizeof(p_err_menu), BLOCKING);
}
  
  if (bLSErrType & UART_LSR_FE)
  {
  UART_Send(LPC_UART0, f_err_menu, sizeof(f_err_menu), BLOCKING);
}
  */
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
* @brief    UART read function for interrupt mode (using ring buffers)
* @param[in]  UARTPort  Selected UART peripheral used to send data,
*         should be UART0
* @param[out]  rxbuf Pointer to Received buffer
* @param[in]  buflen Length of Received buffer
* @return     Number of bytes actually read from the ring buffer
**********************************************************************/
uint32_t UARTReceive1(UART_ID_Type UartID, uint8_t *rxbuf, uint8_t buflen)
{
  uint8_t *data = (uint8_t *) rxbuf;
  uint32_t bytes = 0;
  
  /* Temporarily lock out UART receive interrupts during this
  read so the UART receive interrupt won't cause problems
  with the index values */
  UART_IntConfig(UART_0, UART_INTCFG_RBR, DISABLE);
  
  /* Loop until receive buffer ring is empty or
  until max_bytes expires */
  while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb1.rx_head, rb1.rx_tail))))
  {
    /* Read data from ring buffer into user buffer */
    *data = rb1.rx[rb1.rx_tail];
    data++;
    
    /* Update tail pointer */
    __BUF_INCR(rb1.rx_tail);
    
    /* Increment data count and decrement buffer size count */
    bytes++;
    buflen--;
  }
  
  /* Re-enable UART interrupts */
  UART_IntConfig(UartID, UART_INTCFG_RBR, ENABLE);
  
  return bytes;
}

