#include "RelayController.h"
#include "lpc177x_8x_mci.h"


const PIN_ARRAY m_PinArray[17] = {
  {0, 0 , 0, 0},   //0
  {1, 31, 1, 30},  //1
  {0, 28, 0, 13},  //2
  {0, 12, 2, 26},  //3
  {0, 0 , 0, 0},   //4
  {0, 0 , 0, 0},   //5
  {1, 19 , 1, 20}, //6
  {4, 17 , 4, 18}, //7
	{2, 13, 2, 15},   //8
  {2, 12 , 0, 22},   //9
  {2, 27 , 2, 25},   //10
  {0, 31 , 0, 27},   //11
  {0, 29 , 2, 23},   //12
  {1, 27 , 2, 14},   //13
  {1, 28 , 2, 11},   //14
  {4, 21 , 0, 19},   //15
  {0, 0  , 0, 0}    //16
};

void Put_OBDPin_Function(uint8_t obd_pin)
{
  PINSEL_ConfigPin(m_PinArray[obd_pin].pin_rx_port,m_PinArray[obd_pin].pin_rx_num,0);
  GPIO_SetDir(m_PinArray[obd_pin].pin_rx_port,(1 << m_PinArray[obd_pin].pin_rx_num),1);
  
  PINSEL_ConfigPin(m_PinArray[obd_pin].pin_tx_port,m_PinArray[obd_pin].pin_tx_num,0);
  GPIO_SetDir(m_PinArray[obd_pin].pin_tx_port,(1 << m_PinArray[obd_pin].pin_tx_num),1);
}


void Activate_OBDPin(uint8_t obd_pin, uint8_t condition)
{
  Put_OBDPin_Function(obd_pin);
	
		//this if needed for pin12 (p0.29) which shares a direction bit with p0.30
	if(obd_pin==12)
	{
	 PINSEL_ConfigPin(m_PinArray[obd_pin].pin_rx_port,m_PinArray[obd_pin].pin_rx_num+1,0);
   GPIO_SetDir(m_PinArray[obd_pin].pin_rx_port,(1 << (m_PinArray[obd_pin].pin_rx_num+1)),1);
	}
  
  
  switch (condition)
  {
  case OFF_MODE:
    GPIO_ClearValue(m_PinArray[obd_pin].pin_rx_port,(1 << m_PinArray[obd_pin].pin_rx_num));
    GPIO_ClearValue(m_PinArray[obd_pin].pin_tx_port,(1 << m_PinArray[obd_pin].pin_tx_num));

    break;
    
  case ON_MODE:
    GPIO_SetValue(m_PinArray[obd_pin].pin_rx_port,(1 << m_PinArray[obd_pin].pin_rx_num));
    GPIO_SetValue(m_PinArray[obd_pin].pin_tx_port,(1 << m_PinArray[obd_pin].pin_tx_num));

    break;
    
  case ON_RX_MODE:
    GPIO_SetValue(m_PinArray[obd_pin].pin_rx_port,(1 << m_PinArray[obd_pin].pin_rx_num));

    break;
    
  case ON_TX_MODE:
    GPIO_SetValue(m_PinArray[obd_pin].pin_tx_port,(1 << m_PinArray[obd_pin].pin_tx_num));

    break;
   
  };
  
}

void Activate_LEDPin(uint8_t led_num, uint8_t condition)
{
  uint8_t led_pin = 0;
  uint8_t led_port = 0;
  
  switch (led_num)
  {
  case LED_FAULT:
    led_pin =   LED_FAULT_PIN;
    led_port =  LED_FAULT_PORT; 
    break;
    
  case LED_BUZZER:
    led_pin = BUZZER_PIN;
    led_port = BUZZER_PORT; 
    break;
    
  case LED_CONNECT:
    led_pin = LED4_CONNECT_PIN;
    led_port =LED4_CONNECT_PORT;
    break;
  }
  
  
  PINSEL_ConfigPin(led_port,led_pin,0);
  GPIO_SetDir(led_port,(1 << led_pin),1);
  
  
  switch (condition)
  {
  case OFF_MODE:
    GPIO_ClearValue(led_port,(1 << led_pin));

    break;
    
  case ON_MODE:
    GPIO_SetValue(led_port,(1 << led_pin));

    break;

  };
  
}

void Activate_ProtocolPin(uint8_t led_num, uint8_t condition)
{
  uint8_t led_pin = 0;
  uint8_t led_port = 0;
  
  switch (led_num)
  {
  case PROTOCOL_KWP_RELAY:
    led_pin =   PRPTOCOL_KWP_PIN;
    led_port =  PROTOCOL_KWP_PORT; 
    break;
    
  case PROTOCOL_CAN_RELAY:
    led_pin = PRPTOCOL_CAN_PIN;
    led_port = PROTOCOL_CAN_PORT; 
    break;
    
  case PROTOCOL_TYPE1_RELAY:
    led_pin = PROTOCOL_1_PIN;
    led_port =PROTOCOL_1_PORT;
    break;
  }
  //set the function to 0:
  PINSEL_ConfigPin(led_port,led_pin,0);
  //set to the output:
  GPIO_SetDir(led_port,(1 << led_pin),1);
  
  
  switch (condition)
  {
  case OFF_MODE:
    GPIO_ClearValue(led_port,(1 << led_pin));
    
    break;
    
  case ON_MODE:
    GPIO_SetValue(led_port,(1 << led_pin));
    
    break;

  };
  
}




    
void TrunOnWriteProtectionNAND_Module(uint8_t condition)
{
  uint8_t led_port = WP_FLASH_PORT;
  uint8_t led_pin = WP_FLASH_PIN;
  
  PINSEL_ConfigPin(led_port,led_pin,0);
  GPIO_SetDir(led_port,(1 << led_pin),1);
  
  if(condition == OFF_MODE)
  {
    GPIO_SetValue(led_port,(1 << led_pin));
  }
  else
  {
    GPIO_ClearValue(led_port,(1 << led_pin));
  }
  
}

void NFR_NAND_Module(void)
{
  uint8_t led_port = WP_FLASH_PORT;
  uint8_t led_pin = WP_FLASH_PIN;
  
  PINSEL_ConfigPin(led_port,led_pin,0);
  GPIO_SetDir(led_port,(1 << led_pin),0);
 
}



///////////////////////////// MCI(SDCARD) MODULE //////////////////
void TurnOnSDCARD_Module(uint8_t condition)
{
  uint8_t led_port = P1_5_MICPWR_PORT;
  uint8_t led_pin = P1_5_MICPWR_PIN;
  
  PINSEL_ConfigPin(led_port,led_pin,0);
  GPIO_SetDir(led_port,(1 << led_pin),1);
  
  if(condition == OFF_MODE)
  {
    MCI_PowerOff();
  }
  else if(condition == ON_MODE)
  {
    GPIO_ClearValue(led_port,(1 << led_pin));
  }
  else
  {
   while(1);
  }
  
}




void Activate_XORPin(uint8_t xor_num, uint8_t condition)
{
  uint8_t led_pin = 0;
  uint8_t led_port = 0;
  
  switch (xor_num)
  {
  case XOR_RX:
    led_pin =   XOR_RX_PIN;
    led_port =  XOR_RX_PORT; 
    break;
    
  case XOR_TX:
    led_pin = XOR_TX_PIN;
    led_port = XOR_TX_PORT; 
    break;
  }
  
  
  PINSEL_ConfigPin(led_port,led_pin,0);
  GPIO_SetDir(led_port,(1 << led_pin),1);
  
  
  switch (condition)
  {
  case OFF_MODE:
    GPIO_ClearValue(led_port,(1 << led_pin));

    break;
    
  case ON_MODE:
    GPIO_SetValue(led_port,(1 << led_pin));

    break;

  };
  
}

char CheckSDCardAvailable(void)
{
  //P1_18_CARDDETECT_PORT
  //P1_18_CARDDETECT_PIN
  uint32_t value_port = GPIO_ReadValue(P1_18_CARDDETECT_PORT);
  
  if( (value_port &=(1<<P1_18_CARDDETECT_PIN)) )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
//PIN1_SWITCH_DETECT_PORT,PIN1_SWITCH_DETECT_PIN
char pin1_switch_detected(void)
{
	uint32_t pvalue;
  pvalue = GPIO_ReadValue(PIN1_SWITCH_DETECT_PORT);
	if((pvalue & 0x02) !=0)
	{
		//detected
	return(1);
	}
	//not detected
  return(0);
}




