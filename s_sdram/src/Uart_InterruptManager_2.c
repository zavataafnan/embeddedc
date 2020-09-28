
#include "stdint.h"
#include "stdlib.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc_types.h"
#include "Uart_InterruptManager.h"
#include "Uart_InterruptManager_2.h"
#include "globalDef.h"
#include "MbCRC.h"
#include "Uart_Helper.h"
#include "CommProtocol.h"
#include "ErrHandling.h"
#include "CRCGenerator.h"
#include "Protocol_KWP.h"



/************************** PRIVATE VARIABLES *************************/
// UART Ring buffer
UART_RING_BUFFER_T rb2;
uint8_t m_SendStateForUart2;
uint16_t m_NumberOfSendBytes;
volatile uint8_t KWPRxCountFlag;

extern ProtocolProfiler * m_CurrentProf;
char check_For_rec = 0;
volatile char reflect_condition = 0;
/*----------------- INTERRUPT SERVICE ROUTINES FOR UART 2--------------------------*/
/*********************************************************************
* UART1 interrupt handler sub-routine
**********************************************************************/
void UART2_IRQHandler(void)
{
  uint32_t intsrc, tmp, tmp1;
  
  /* Determine the interrupt source */
  intsrc = UART_GetIntId(UART_2);
  tmp = intsrc & UART_IIR_INTID_MASK;
  
  // Receive Line Status
  if (tmp == UART_IIR_INTID_RLS)
  {
    // Check line status
    tmp1 = UART_GetLineStatus(UART_2);
    // Mask out the Receive Ready and Transmit Holding empty status
    tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
    // If any error exist
    if (tmp1)
    {
      UART_IntErr(tmp1);
    }
  }
  
  // Receive Data Available or Character time-out
  if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
  {
    UART_IntReceive2(UART_2);
  }
  
}

void Reset_UartRingBuffer2(void)
{
  //	int indxI;
  __BUF_RESET(rb2.rx_head);
  __BUF_RESET(rb2.rx_tail);
  __BUF_RESET(rb2.tx_head);
  __BUF_RESET(rb2.tx_tail);
  
  //	for (indxI = 0; indxI < UART_RING_BUFSIZE; indxI++)
  //	{
  //		rb2.rx[indxI] = 0xFF;
  //		rb2.tx[indxI] = 0xFF;
  //	}
}

/********************************************************************
* UART receive function (ring buffer used)
*********************************************************************/
void UART_IntReceive2(UART_ID_Type UartID)
{
  int len = 0;
  int_char_little dump_int_to_char;
  uint8_t mbFunc = 0;
  uint8_t buffer_rec[50];
  //	uint16_t regAddr = 0xFFFF;
  uint8_t tmpc[5];
  uint32_t rLen;
  uint16_t num_of_byte = 0;
  uint16_t crc_check = 0;
  char wait_condition =0;

    // Call UART read function in UART driver
    rLen = UART_Receive(UartID, tmpc, 1, NONE_BLOCKING, 500);
    if(rb2.rx_head == 1 && tmpc == 0x00)
    {
      Reset_UartRingBuffer2();
      return;
    }
    
    if (!rLen) // no more data v 
      return;
    
    // If data received
    
    /* Check if buffer is more space
    * If no more space, remaining character will be trimmed out
    */
      if (!__BUF_IS_FULL(rb2.rx_head,rb2.rx_tail))
      {
        rb2.rx[rb2.rx_head] = tmpc[0];
        __BUF_INCR(rb2.rx_head);
        
        if(m_NumberOfSendBytes == rb2.rx_head)
        {
          Reset_UartRingBuffer2();
          m_NumberOfSendBytes = 0xffff;
					check_For_rec = 1;
        }
        
				if(m_CurrentProf->protocol_type == PROTOCOL_KWP)
				{
					
        if(m_CurrentProf->protocol_solver->isReadyFrame(rb2.rx,rb2.rx_head) && (check_For_rec == 1))
        {
          int y = 0;
          //copy the buffer ....
          // set the flag that the data was recieved successfully...
          // reset the buffer ....
					
					//m_CurrentProf->protocol_solver->
						if(m_CurrentProf->spect->Kwp_spect->protocol_type == KWP_TYPE_IMMO)
						{
								if(rb2.rx[1] == 0x7f && rb2.rx[3] == 0x78)
								wait_condition = 1;		
						}
						else
						{
		if(rb2.rx[0] > 0x80)
		{
			if(rb2.rx[3] == 0x7f && rb2.rx[5] == 0x78)
				wait_condition = 1;
		} else if (rb2.rx[0] == 0x80)
		{
			if(rb2.rx[4] == 0x7f && rb2.rx[6] == 0x78)
				wait_condition = 1;
		}
	}
	
	
          if(wait_condition == 1)
					{
						Reset_UartRingBuffer2();
						delay_ms_interrupt(m_CurrentProf->spect->Kwp_spect->serial_port_cofig->TimeOut);	
					}
					else
						KWPRxCountFlag = 1;
          
          //Reset_UartRingBuffer2();
        }
				
			}
        
        
        
      }
}


/*********************************************************************
* @brief    UART Line Status Error
* @param[in]  bLSErrType  UART Line Status Error Type
* @return    None
**********************************************************************/
void UART_IntErr2(uint8_t bLSErrType)
{
  /*
  if (bLSErrType & UART_LSR_PE)
  {
  UART_Send(LPC_UART2, p_err_menu, sizeof(p_err_menu), BLOCKING);
}
  
  if (bLSErrType & UART_LSR_FE)
  {
  UART_Send(LPC_UART2, f_err_menu, sizeof(f_err_menu), BLOCKING);
}
  */
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
* @brief    UART read function for interrupt mode (using ring buffers)
* @param[in]  UARTPort  Selected UART peripheral used to send data,
*         should be UART2
* @param[out]  rxbuf Pointer to Received buffer
* @param[in]  buflen Length of Received buffer
* @return     Number of bytes actually read from the ring buffer
**********************************************************************/
uint32_t UARTReceive2(UART_ID_Type UartID, uint8_t *rxbuf, uint8_t buflen)
{
  uint8_t *data = (uint8_t *) rxbuf;
  uint32_t bytes = 0;
  
  /* Temporarily lock out UART receive interrupts during this
  read so the UART receive interrupt won't cause problems
  with the index values */
  UART_IntConfig(UartID, UART_INTCFG_RBR, DISABLE);
  
  /* Loop until receive buffer ring is empty or
  until max_bytes expires */
  while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb2.rx_head, rb2.rx_tail))))
  {
    /* Read data from ring buffer into user buffer */
    *data = rb2.rx[rb2.rx_tail];
    data++;
    
    /* Update tail pointer */
    __BUF_INCR(rb2.rx_tail);
    
    /* Increment data count and decrement buffer size count */
    bytes++;
    buflen--;
  }
  
  /* Re-enable UART interrupts */
  UART_IntConfig(UartID, UART_INTCFG_RBR, ENABLE);
  
  return bytes;
}

