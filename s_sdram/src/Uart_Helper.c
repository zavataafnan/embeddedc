#include "uart_helper.h"
#include "Uart_InterruptManager.h"
#include "Uart_InterruptManager_1.h"
#include "log_Helper.h"
#include "CommProtocol.h"
/******************************************************************************
* Main method
*****************************************************************************/


//for DMA mode

/* Receive buffer size */
#define RX_BUF_SIZE	0x10

extern uint8_t m_LogEnabled;
extern SCHEDULER_INPUT_MANAGEMENT m_Scheduler_Buffer;

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"Hello NXP Semiconductors \n\r"
"UART interrupt mode demo using ring buffer \n\r\t "
"MCU LPC17xx - ARM Cortex-M3 \n\r\t "
"UART0 - 9600bps \n\r"
" This is a long string. It transferred in to DMA memory and transmit through Tx line \n\r"
" on UART0 peripheral. To use UART with DMA mode, FIFO function must be enabled \n\r";

uint8_t menu3[] = "UART demo terminated!\n";

// Receive buffer
__IO uint8_t rx_buf[RX_BUF_SIZE];

// Terminal Counter flag for Channel 0
__IO uint32_t Channel0_TC;

// Error Counter flag for Channel 0
__IO uint32_t Channel0_Err;




int Initialize_UART_NoDMA(UART_ID_Type UartID)
{
  // UART Configuration structure variable
  UART_CFG_Type UARTConfigStruct;
  // UART FIFO configuration Struct variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;
  GPDMA_Channel_CFG_Type GPDMACfg;
  // Pin configuration for UART0
  //PINSEL_CFG_Type PinCfg;
  
  /*
  * Initialize UART0 pin connect
  */
  
  //void PINSEL_ConfigPin(uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
  
  PINSEL_ConfigPin(0, 2, 1);
  PINSEL_ConfigPin(0, 3, 1);
  
  //PINSEL_ConfigPin(&PinCfg);
  
  /* Initialize UART Configuration parameter structure to default state:
  * Baudrate = 9600bps
  * 8 data bit
  * 1 Stop bit
  * None parity
  */
  UART_ConfigStructInit(&UARTConfigStruct);
  
  // Initialize UART0 peripheral with given to corresponding parameter
  UART_Init(UART_0, &UARTConfigStruct);
  
  
  /* Initialize FIFOConfigStruct to default state:
  * 				- FIFO_DMAMode = DISABLE
  * 				- FIFO_Level = UART_FIFO_TRGLEV0
  * 				- FIFO_ResetRxBuf = ENABLE
  * 				- FIFO_ResetTxBuf = ENABLE
  * 				- FIFO_State = ENABLE
  */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  
  
  
  // Initialize FIFO for UART0 peripheral
  UART_FIFOConfig(UartID, &UARTFIFOConfigStruct);
  
  // Enable UART Transmit
  //UART_TxCmd(LPC_UART0, ENABLE);
  
  
  if (UartID == UART_0)
  {
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART0_IRQn, ((0x01 << 3) | 0x01));
    /* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);
  }
  
  
  if (UartID == UART_1)
  {
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART1_IRQn, ((0x01 << 3) | 0x01));
    /* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART1_IRQn);
  }
  
  return 0;
}



int Initialize_UART_WithTXDMA(LPC_UART_TypeDef* UARTPort, uint8_t Buffer_tx[], uint16_t size_buffer_tx)
{
  // UART Configuration structure variable
  UART_CFG_Type UARTConfigStruct;
  // UART FIFO configuration Struct variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;
  GPDMA_Channel_CFG_Type GPDMACfg;
  // Pin configuration for UART0
  
  
  /*
  * Initialize UART0 pin connect
  */
  //PinCfg.Funcnum = 1;
  //PinCfg.OpenDrain = 0;
  //PinCfg.Pinmode = 0;
  //PinCfg.Pinnum = 2;
  //PinCfg.Portnum = 0;
  //void PINSEL_ConfigPin(uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
  
  //PINSEL_ConfigPin(PinCfg.Portnum, PinCfg.Pinnum, PinCfg.Funcnum);
  PINSEL_ConfigPin(0,2,1);
  //PinCfg.Pinnum = 3;
  PINSEL_ConfigPin(0,3,1);
  //PINSEL_ConfigPin(PinCfg.Portnum, PinCfg.Pinnum, PinCfg.Funcnum);
  //PINSEL_ConfigPin(&PinCfg);
  
  /* Initialize UART Configuration parameter structure to default state:
  * Baudrate = 9600bps
  * 8 data bit
  * 1 Stop bit
  * None parity
  */
  UART_ConfigStructInit(&UARTConfigStruct);
  
  // Initialize UART0 peripheral with given to corresponding parameter
  UART_Init(UART_0, &UARTConfigStruct);
  
  
  /* Initialize FIFOConfigStruct to default state:
  * 				- FIFO_DMAMode = DISABLE
  * 				- FIFO_Level = UART_FIFO_TRGLEV0
  * 				- FIFO_ResetRxBuf = ENABLE
  * 				- FIFO_ResetTxBuf = ENABLE
  * 				- FIFO_State = ENABLE
  */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  
  // Enable DMA mode in UART
  UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;
  
  // Initialize FIFO for UART0 peripheral
  UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);
  
  // Enable UART Transmit
  //UART_TxCmd(LPC_UART0, ENABLE);
  
  /* GPDMA Interrupt configuration section ------------------------------------------------- */
  
  /* Initialize GPDMA controller */
  GPDMA_Init();
  
  
  /* Setting GPDMA interrupt */
  // Disable interrupt for DMA
  NVIC_DisableIRQ (DMA_IRQn);
  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
  
  
  // Setup GPDMA channel --------------------------------
  // channel 0
  GPDMACfg.ChannelNum = 0;
  // Source memory
  GPDMACfg.SrcMemAddr = (uint32_t) &Buffer_tx;
  // Destination memory - don't care
  GPDMACfg.DstMemAddr = 0;
  // Transfer size
  GPDMACfg.TransferSize = size_buffer_tx;
  // Transfer width - don't care
  GPDMACfg.TransferWidth = 0;
  // Transfer type
  GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
  // Source connection - don't care
  GPDMACfg.SrcConn = 0;
  // Destination connection
  GPDMACfg.DstConn = GPDMA_CONN_UART0_Tx;
  // Linker List Item - unused
  GPDMACfg.DMALLI = 0;
  // Setup channel with given parameter
  GPDMA_Setup(&GPDMACfg);
  
  /*
  // Setup GPDMA channel --------------------------------
  // channel 1
  GPDMACfg.ChannelNum = 1;
  // Source memory - don't care
  GPDMACfg.SrcMemAddr = 0;
  // Destination memory
  GPDMACfg.DstMemAddr = (uint32_t) &rx_buf;
  // Transfer size
  GPDMACfg.TransferSize = sizeof(rx_buf);
  // Transfer width - don't care
  GPDMACfg.TransferWidth = 0;
  // Transfer type
  GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;
  // Source connection
  GPDMACfg.SrcConn = GPDMA_CONN_UART0_Rx;
  // Destination connection - don't care
  GPDMACfg.DstConn = 0;
  // Linker List Item - unused
  GPDMACfg.DMALLI = 0;
  GPDMA_Setup(&GPDMACfg);
  */
  
  
  /* Reset terminal counter */
  Channel0_TC = 0;
  /* Reset Error counter */
  Channel0_Err = 0;
  
  // Enable interrupt for DMA
  NVIC_EnableIRQ (DMA_IRQn);
  
  // disable GPDMA channel 0
  GPDMA_ChannelCmd(0, DISABLE);
  // Make sure GPDMA channel 1 is disabled
  //GPDMA_ChannelCmd(1, DISABLE);
  
  /* Wait for GPDMA on UART0 Tx processing complete */
  // while ((Channel0_TC == 0) && (Channel0_Err == 0));
  
  
  
  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(UART0_IRQn, ((0x01 << 3) | 0x01));
  /* Enable Interrupt for UART0 channel */
  NVIC_EnableIRQ(UART0_IRQn);
  
  UART_TxCmd(UART_0, ENABLE);
  
  return 0;
  
}


void Server_Port_Config0(void)
{
  
  UART_CFG_Type UARTConfigStruct;						// UART Configuration structure variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
  
  
  // UART0 section ----------------------------------------------------
  /*
  * Initialize UART0 pin connect
  */
  
  /*
  * Initialize UART0 pin connect
  */
  
  //void PINSEL_ConfigPin(uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
  
  PINSEL_ConfigPin(0, 2, 1);
  PINSEL_ConfigPin(0, 3, 1);
  
  //PINSEL_ConfigPin(&PinCfg);
  
  
  /* Initialize UART Configuration parameter structure to default state:
  * Baudrate = 9600 bps
  * 8 data bit
  * 1 Stop bit
  * None parity
  */
  UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = 115200;
  
  // Initialize UART0 peripheral with given to corresponding parameter
  UART_Init(UART_0, &UARTConfigStruct);
  
  /* Initialize FIFOConfigStruct to default state:
  *	- FIFO_DMAMode = DISABLE
  *	- FIFO_Level = UART_FIFO_TRGLEV0
  *	- FIFO_ResetRxBuf = ENABLE
  *	- FIFO_ResetTxBuf = ENABLE
  *	- FIFO_State = ENABLE
  */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  
  // Initialize FIFO for UART0 peripheral
  UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);
  
  // Enable UART Transmit
  UART_TxCmd(UART_0, ENABLE);
  
  /* Enable UART Rx interrupt */
  UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
  /* Enable UART line status interrupt */
  UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
  /*
  * Do not enable transmit interrupt here, since it is handled by
  * UART_Send() function, just to reset Tx Interrupt state for the
  * first time
  */
  
  Reset_UartRingBuffer();
  
  
  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(UART0_IRQn, ((0x01 << 3) | 0x01));
  /* Enable Interrupt for UART0 channel */
  NVIC_EnableIRQ(UART0_IRQn);
}




void Server_Port_Config1(int baud_rate)
{
  
  UART_CFG_Type UARTConfigStruct;						// UART Configuration structure variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
  
  
  // UART0 section ----------------------------------------------------
  /*
  * Initialize UART0 pin connect
  */
  
  /*
  * Initialize UART0 pin connect
  */
  
  //void PINSEL_ConfigPin(uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
  
//  PINSEL_ConfigPin(0, 15, 1);
//  PINSEL_ConfigPin(0, 16, 1);
   
  PINSEL_ConfigPin(2, 0, 2);
  PINSEL_ConfigPin(2, 1, 2);
  
  
  //PINSEL_ConfigPin(&PinCfg);
  
  
  /* Initialize UART Configuration parameter structure to default state:
  * Baudrate = 9600 bps
  * 8 data bit
  * 1 Stop bit
  * None parity
  */
  UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = baud_rate;
  
  // Initialize UART0 peripheral with given to corresponding parameter
  //UART_Init((LPC_UART_TypeDef*)LPC_UART1, &UARTConfigStruct);
  UART_Init(UART_1, &UARTConfigStruct);
  /* Initialize FIFOConfigStruct to default state:
  *	- FIFO_DMAMode = DISABLE
  *	- FIFO_Level = UART_FIFO_TRGLEV0
  *	- FIFO_ResetRxBuf = ENABLE
  *	- FIFO_ResetTxBuf = ENABLE
  *	- FIFO_State = ENABLE
  */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  
  // Initialize FIFO for UART0 peripheral
  //UART_FIFOConfig((LPC_UART_TypeDef*)LPC_UART1, &UARTFIFOConfigStruct);
  UART_FIFOConfig(UART_1, &UARTFIFOConfigStruct);
  // Enable UART Transmit
  //UART_TxCmd((LPC_UART_TypeDef*)LPC_UART1, ENABLE);
  UART_TxCmd(UART_1, ENABLE);
  
  /* Enable UART Rx interrupt */
  //UART_IntConfig((LPC_UART_TypeDef*)LPC_UART1, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig(UART_1, UART_INTCFG_RBR, ENABLE);
  /* Enable UART line status interrupt */
  //UART_IntConfig((LPC_UART_TypeDef*)LPC_UART1, UART_INTCFG_RLS, ENABLE);
  UART_IntConfig(UART_1, UART_INTCFG_RLS, ENABLE);
  
  /*
  * Do not enable transmit interrupt here, since it is handled by
  * UART_Send() function, just to reset Tx Interrupt state for the
  * first time
  */
  
  Reset_UartRingBuffer1();
  
  
  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(UART1_IRQn, ((0x01 << 3) | 0x01));
  /* Enable Interrupt for UART0 channel */
  NVIC_EnableIRQ(UART1_IRQn);
}


void Server_Port_Config2(int baud_rate)
{
  
  UART_CFG_Type UARTConfigStruct;						// UART Configuration structure variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
  
 
  
  PINSEL_ConfigPin(0, 10, 1);
  PINSEL_ConfigPin(0, 11, 1);
  
  //PINSEL_ConfigPin(&PinCfg);
  

  UART_ConfigStructInit(&UARTConfigStruct);
  UARTConfigStruct.Baud_rate = baud_rate;
  
  // Initialize UART0 peripheral with given to corresponding parameter
  UART_Init(UART_2, &UARTConfigStruct);
  
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
  
  // Initialize FIFO for UART0 peripheral
  UART_FIFOConfig(UART_2, &UARTFIFOConfigStruct);
  
  // Enable UART Transmit
  UART_TxCmd(UART_2, ENABLE);
  

  UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig(UART_2, UART_INTCFG_RLS, ENABLE);

  
  Reset_UartRingBuffer2();
  
  

  NVIC_SetPriority(UART2_IRQn, ((0x01 << 3) | 0x01));
 
  NVIC_EnableIRQ(UART2_IRQn);
}



void Write_ModeBusFrame(UART_ID_Type UartID, uint8_t* pyFrame, int nNumByte)
{
  LPC_UART_TypeDef * uart = LPC_UART0;
  UART_TxCmd(UartID, ENABLE);
	
  
  //delay_ms(5);
  //delay_us(10); 

  UART_Send(UartID, pyFrame, nNumByte, BLOCKING);
  
  
  switch (UartID)
  {
  case UART_0:
    uart = LPC_UART0;
    break;
  case UART_1:
    uart = (LPC_UART_TypeDef*)LPC_UART1;
    break;	
  case UART_2:
    uart = LPC_UART2;
    break;
  case UART_3:
    uart = LPC_UART3;
    break;
  case UART_4:
    uart = (LPC_UART_TypeDef*)LPC_UART4;
    break;
  }
  
  while (!(uart->LSR & UART_LSR_TEMT));
  
}

void Write_ModeBusFrame_WithReflect(UART_ID_Type UartID, uint8_t* pyFrame, int nNumByte)
{
  LPC_UART_TypeDef * uart = LPC_UART0;
  UART_TxCmd(UartID, ENABLE);
  
  //delay_ms(5);
  //delay_us(10); 
	
	
  UART_Send_WithReflect(UartID, pyFrame, nNumByte, BLOCKING);
  
  
  switch (UartID)
  {
  case UART_0:
    uart = LPC_UART0;
    break;
  case UART_1:
    uart = (LPC_UART_TypeDef*)LPC_UART1;
    break;	
  case UART_2:
    uart = LPC_UART2;
    break;
  case UART_3:
    uart = LPC_UART3;
    break;
  case UART_4:
    uart = (LPC_UART_TypeDef*)LPC_UART4;
    break;
  }
  
  while (!(uart->LSR & UART_LSR_TEMT));
  
}

