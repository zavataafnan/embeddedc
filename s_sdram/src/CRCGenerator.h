#pragma once
#include "globalDef.h"

#define FLAG_CRC_POLY_CRCCCITT 0
#define FLAG_CRC_POLY_CRC16    1
#define FLAG_CRC_POLY_CRC32    2

uint8_t isCheckSumCorrect(uint8_t * tx);
