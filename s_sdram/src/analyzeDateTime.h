#ifndef __ANALYZE_TIME__
#define __ANALYZE_TIME__
#include "globalDef.h"
#include "CommProtocol.h"
#include "lpc177x_8x_rtc.h"
unsigned char AnalyzeFrameChangeTime(m_DateFrame * dfrm);
unsigned char AnalyzeFrameReadDateTime(RTC_TIME_Type * rtcful);

#endif
