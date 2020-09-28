#pragma once
#include "globalDef.h"
#include "CommProtocol.h"

#define LEVEL_1_BOOTLOADER_ACCESS       1001
#define LELEL_2_FLASH_ACCESS            1002
#define LEVEL_3_SDCARD_ACCESS           1003
#define LEVEL_4_EEPROM_ACCESS           1004
#define LEVEL_5_FTDI_ACCESS             1005
#define LEVEL_6_NORMAL_ACCESS           1006
#define LEVEL_0_NO_ACCESS               1007
#define LEVEL_FIRST_ACCESS              1008
#define LEVEL_RECIEVING_LICENSES        1009


typedef struct _security_limit_
{
  uint16_t m_number_of_CRC_error_counter;
  uint16_t m_number_of_reset_CRC_counter;
} m_SecurityLimits;


typedef struct _check_information_
{
  m_DateFrame m_CheckDates;
  uint8_t m_Banks;
  char m_SerialCheck[15];
  char status;
  
} m_CheckInformation;


typedef struct _block_conditions_
{
  uint8_t m_CRCErrorBlock;
  uint8_t m_CounterBlock;
  m_CheckInformation m_CheckInfo[10];
} m_Blocking_Conditions;

void CheckTheDeviceForLocking(void);


typedef struct _LAST_WRITTEN_SECTION_
{
		uint16_t nblock;
		uint8_t section;
} LAST_WRITTEN_VALUES;

typedef struct _JUMP_CONNECTION_
{
	uint8_t m_ConnectCondition;
	uint16_t m_ECU_requested;
	uint8_t m_StatusCondition;
} JUMP_CONNECTION;

typedef struct _CALIBRATION_STRUCTURE
 {
    float bat_offset; // Lot Number Byte 0, ASCII
    float bat_coef; // Lot Number Byte 1, ASCII
    float voltage_offset; // Lot Number Byte 2, ASCII
    float voltage_coef; // Lot Number Byte 3, ASCII
  } CALIBRATION_VALUE;
 
	

typedef struct _security_ID_
{
  char m_SerialNumber[16];
  unsigned char m_EEPROM[6];
  unsigned char m_FLASH[6];
  unsigned char m_SDCard[4];
  unsigned char XMEGA[6];
  unsigned char m_FTDI[6];
	unsigned int access_level;
	uint8_t initialize_setup;
	uint8_t status;
	uint8_t reasons_of_blocks;
	uint8_t reasons_of_blocks_server;
	uint32_t bluetooth_baudrate;
	uint16_t m_RecCounter;
	uint16_t m_SendCounter;
	LAST_WRITTEN_VALUES last_written_values;
	uint8_t session_status;
	char last_time_turning_on[15];
	uint8_t m_ConnectCondition;
	uint16_t m_ECU_requested;
	TIME_DATE start_Of_activation_process_time;
	TIME_DATE last_run_process_time;
	uint16_t number_of_Day_to_check_Activate;
	CALIBRATION_VALUE cal_val;
	uint8_t reserved[10];

} SECURITY_ID;





