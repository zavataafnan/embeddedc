#ifndef _BT_HELPER__
#define _BT_HELPER__
#include "stdint.h"
void SetBluetooth_Command(uint32_t baud_rate);
void BluetoothControlPin(uint8_t condition);
void Bluetooth_Power(uint8_t condition);
void Bluetooth_Set_Mode(uint8_t condition);
#endif