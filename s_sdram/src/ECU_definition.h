#ifndef __ECI_DEF___
#define __ECI_DEF___

//#include "E000.h"
#include "globalDef.h"
#define NumberOfECU 1

#define E000 0

#define SCHEDULER_STATUS_GO_TO_DEFAULT  0x10
#define SCHEDULER_STATUS_END 0x11
#define SCHEDULER_STATUS_CON 0x00

#define XOR_RXD_PORT 0
#define XOR_TXD_PORT 0
#define XOR_RXD_PIN  5
#define XOR_TXD_PIN  7

#define U2_UART_PORT 0

#define U2_UART_TXD 10
#define U2_UART_RXD 11

#define KWP_TYPE_SHORT 0
#define KWP_TYPE_LONG  1
#define KWP_TYPE_IMMO  2

#define L_LINE_ON     0
#define L_LINE_OFF    1

#define ECU_SL96   0
#define ECU_ABS    1
#define ECU_RIO    2
#define ECU_RIO_ABS 3
#define ECU_ABS_MK  4
 

#define TYPE_CONNECTION_SHORT 0
#define TYPE_CONNECTION_LONG  1
#define TYPE_CONNECTION_IMMO  2

#define KWP_TYPE_SHORT        0
#define KWP_TYPE_LONG         1
#define KWP_TYPE_IMMO         2
#define KWP_TYPE_SL96         3
#define KWP_TYPE_XANTIA2000   4
#define KWP_TYPE_RIO          5


#define INIT_SHORT_BIT  1
#define INIT_LONG_BIT   2
#define INIT_BOTH_BIT   3

#define INIT_SHORT_TYPE 0
#define INIT_LONG_TYPE  1

#define  ECU_XANTIA 0 


#define NONE_PARITY  0
#define EVEN_PARITY  1
#define ODD_PARITY   2



typedef struct _can_filtering_
{
  uint16_t array_ids[10];
  uint8_t id_number;
} CAN_FILTERING_ARRAY;


typedef struct _can_devices_
{       
  uint16_t ID_part;
  uint8_t split_part;
  uint8_t fill_the_Rest ;
  uint16_t Delay_BetweenFrm;
  uint16_t TimeOut;
	uint8_t low_pin;
  uint8_t high_pin;
	uint32_t baudrate;
  CAN_FILTERING_ARRAY * filter_array;
} Can_Device;
 /*
typedef struct _kwp_devices_
{
  uint8_t ID_part[2];
  uint8_t type_of_protocol;
  uint8_t type_of_connect_type;
  uint8_t type_of_init;
  uint16_t TimeOut;
  uint8_t XOR_RXD;
  uint8_t XOR_TXD;
  uint16_t baud_rate;
  uint8_t rec_pin;
  uint8_t txd_pin;

} Kwp_Device;
*/
typedef struct _SHORT_INITIALIZE_
{
	uint8_t type_of_initialize_connect_frame;
	uint8_t l_line_executor;
	uint8_t l_line_pin;
	uint16_t delay_after_init;
} SHORT_INITIALIZE;

typedef struct _LONG_INITIALIZE_
{
	uint8_t initialize_byte;
	uint8_t recieved_byte[3];
	uint8_t l_line_executor;
	uint8_t init_ecu_type;
  uint8_t l_line_pin;
	uint16_t delay_after_init;
} LONG_INITIALIZE;

typedef struct _PIN_CONFIGURATION_
{
	uint8_t rec_pin;
  uint8_t txd_pin;
	uint8_t XOR_RXD;
  uint8_t XOR_TXD;
	
} PIN_CONFIGURATION;

typedef struct _SERIAL_PORT_CONFIGURATION_
{
	uint16_t baud_rate;
	uint8_t even_parity;
	uint16_t TimeOut;
	
} SERIAL_PORT_CONFIG;

typedef struct _CONNECT_INITIALIZE_
{
	uint8_t protocol_type;
	uint8_t init_variatity;
	uint8_t init_type;
	uint8_t ID_part[2];
	SHORT_INITIALIZE * short_init;
	LONG_INITIALIZE * long_init;
	SERIAL_PORT_CONFIG * serial_port_cofig;
	PIN_CONFIGURATION * pin_config;
} Kwp_Device;


typedef struct Device_Types_
{
	Can_Device * can_spect;
	Kwp_Device * Kwp_spect;

} DEVICE_TYPES;
#endif
