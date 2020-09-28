#ifndef __RELAY_CONTROLLER__
#define __RELEAY_CONTROLLER__

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
#include "ECU_define.h"
#include "spi_helper.h"
#include "sdram.h"
#include "nand.h"

#define OBD_PIN0 0
#define OBD_PIN1 1
#define OBD_PIN2 2
#define OBD_PIN3 3
#define OBD_PIN4 4
#define OBD_PIN5 5
#define OBD_PIN6 6
#define OBD_PIN7 7
#define OBD_PIN8 8
#define OBD_PIN9 9
#define OBD_PIN10 10
#define OBD_PIN11 11
#define OBD_PIN12 12
#define OBD_PIN13 13
#define OBD_PIN14 14
#define OBD_PIN15 15
#define OBD_PIN16 16

#define OFF_MODE 0
#define ON_MODE  1
#define ON_RX_MODE  2
#define ON_TX_MODE  3

#define LED_FAULT   0
#define LED_BUZZER  1
#define LED_CONNECT 2

#define PROTOCOL_KWP_RELAY    0
#define PROTOCOL_CAN_RELAY    1
#define PROTOCOL_TYPE1_RELAY  2

#define XOR_RX  0
#define XOR_TX  1


#define LED_FAULT_PORT          0
#define LED_FAULT_PIN           26

#define BUZZER_PORT             0
#define BUZZER_PIN              25

#define LED4_CONNECT_PORT       0
#define LED4_CONNECT_PIN        23

#define PROTOCOL_KWP_PORT       2    
#define PRPTOCOL_KWP_PIN        22

#define XOR_TX_PORT             0            
#define XOR_TX_PIN              4

#define XOR_RX_PORT             4
#define XOR_RX_PIN              15

#define PROTOCOL_CAN_PORT       0
#define PRPTOCOL_CAN_PIN        20

#define PROTOCOL_1_PORT         0
#define PROTOCOL_1_PIN          21            

#define PIN1_SWITCH_DETECT_PORT         5
#define PIN1_SWITCH_DETECT_PIN          1     

//bluetooth section :
#define PIN_CTRL_BMODE_PORT     1 
#define PIN_CTRL_BMODE_PIN      22

#define CTL_HC05_PWR_PORT       2
#define CTL_HC05_PWR_PIN        4

#define BLUETOOTH_COMMAND_MODE  0
#define BLUETOOTH_AT_MODE_9600  1
#define BLUETOOTH_AT_MODE_38400 2


#define NFR_PORT           1
#define NFR_PIN            24


#define WP_FLASH_PORT           1
#define WP_FLASH_PIN            21


//MCI DETECT!!!
#define P1_18_CARDDETECT_PORT  1
#define P1_18_CARDDETECT_PIN   18

#define P1_5_MICPWR_PORT        1
#define P1_5_MICPWR_PIN         5

//FTDI USB TO SERIAL
#define P2_10_ISP_PORT           2
#define P2_10_ISP_PIN           10




typedef struct __PIN_ARRAY__
{
  uint8_t pin_rx_port;
  uint8_t pin_rx_num;
  uint8_t pin_tx_port;
  uint8_t pin_tx_num;
} PIN_ARRAY;


void Activate_OBDPin(uint8_t obd_pin, uint8_t condition);
void TurnOnSDCARD_Module(uint8_t condition);
void Activate_ProtocolPin(uint8_t led_num, uint8_t condition);
void Bluetooth_Set_Mode(uint8_t condition);
char pin1_switch_detected(void);


#endif