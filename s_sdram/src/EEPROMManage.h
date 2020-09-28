#ifndef __EEPROM_MANAGE__
#define __EEPROM_MANAGE__
#include "SecurityLevels.h"
unsigned char WriteTheSecurityPatchToEEPROM(SECURITY_ID * security_id);
unsigned char EraseTheEEPROMAndFillWithRandom(void);
unsigned char ReadTheSecurityPatchToEEPROM(SECURITY_ID * security_id);
#endif
