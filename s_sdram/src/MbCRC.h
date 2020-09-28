/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * File: $Id: mbcrc.h,v 1.5 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_CRC_H
#define _MB_CRC_H

#include "stdint.h"
#include "GlobalDef.h"

USHORT CalcModBusCRC16( UCHAR * pucFrame, USHORT usLen );
void AppendModBusCRC16( UCHAR * pucFrame, USHORT usLen );

void CalcCorusCRC16(uint8_t * ayData, uint16_t nLen, uint8_t * ayCRC);

#endif
