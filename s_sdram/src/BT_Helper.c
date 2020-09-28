#include "BT_Helper.h"
#include "relayController.h"
#include "Uart_Helper.h"
#include "EEPROMManage.h"
extern SECURITY_ID m_SecurityID;


///////////////////////////// BLUETOOTH MODULE //////////////////
void BluetoothControlPin(uint8_t condition)
{
  uint8_t control_port = PIN_CTRL_BMODE_PORT;
  uint8_t control_pin = PIN_CTRL_BMODE_PIN;
  
  PINSEL_ConfigPin(control_port,control_pin,0);
  GPIO_SetDir(control_port,(1 << control_pin),1);
  
  if(condition == ON_MODE)
  {
    GPIO_SetValue(control_port,(1 << control_pin));
  }
  else
  {
    GPIO_ClearValue(control_port,(1 << control_pin));
  }
}


void Bluetooth_Power(uint8_t condition)
{
  uint8_t power_port = CTL_HC05_PWR_PORT;
  uint8_t power_pin = CTL_HC05_PWR_PIN;
  
  PINSEL_ConfigPin(power_port,power_pin,0);
  GPIO_SetDir(power_port,(1 << power_pin),1);
  
  if(condition == OFF_MODE)
  {
    GPIO_SetValue(power_port,(1 << power_pin));
  }
  else
  {
    GPIO_ClearValue(power_port,(1 << power_pin));
  }
  
}

//in AT modes the bluetooth startup connection spped with serial port is 9600 and 38400 respectively
void Bluetooth_Set_Mode(uint8_t condition)
{
	//turn off bluetooth
  //Bluetooth_Power(OFF_MODE);
  //delay_ms(200);

	
	if(condition==BLUETOOTH_COMMAND_MODE)
	{
	  BluetoothControlPin(OFF_MODE);
		delay_ms(100);
    Bluetooth_Power(ON_MODE);
	}
	else
	if(condition==BLUETOOTH_AT_MODE_9600)
	{
		//Bluetooth_Power(OFF_MODE);
		//delay_ms(100);
	  BluetoothControlPin(OFF_MODE);
		delay_ms(100);
    Bluetooth_Power(ON_MODE);
		delay_ms(1000);
	  BluetoothControlPin(ON_MODE);
	}
  else
	if(condition==BLUETOOTH_AT_MODE_38400)
	{
		Bluetooth_Power(OFF_MODE);
		delay_ms(100);
	  BluetoothControlPin(ON_MODE);
		delay_ms(100);
    Bluetooth_Power(ON_MODE);	
		delay_ms(1000);
	}
	  


}


#ifndef __RAM_MODE__
void SetBluetooth_Command(uint32_t baud_rate)
{
	char AT_command2[] = "AT+UART=921600,0,0\r\n";
	uint8_t At_rec[10];
	
	Bluetooth_Set_Mode(BLUETOOTH_AT_MODE_38400);
	Server_Port_Config1(38400);
	UART_IntConfig(UART_1, UART_INTCFG_RBR, DISABLE);
	
	sprintf(AT_command2, "AT+UART=%d,0,0\r\n", baud_rate);
	
	UART_Send(UART_1, (char *) AT_command2, strlen((char *)AT_command2), BLOCKING);
  UART_Receive(UART_1, At_rec, 4, BLOCKING, 250);
	
	if(At_rec[0] == 'O' && At_rec[1] == 'K')
	{
		Server_Port_Config1(baud_rate);
		//m_SecurityID.bluetooth_baudrate = baud_rate;
		//WriteTheSecurityPatchToEEPROM(&m_SecurityID);
	}
	
	
	Bluetooth_Power((uint8_t)OFF_MODE);
	delay_ms(100);
	
	Bluetooth_Set_Mode(BLUETOOTH_COMMAND_MODE);
}
#endif



