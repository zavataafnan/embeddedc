#include "lpc177x_8x_libcfg_default.h"
#include "analyzeDateTime.h"
#include "CommProtocol.h"
#include "lpc177x_8x_rtc.h"
#include "debug_frmwrk.h"

/////////////////////////////////////////////////////
unsigned char AnalyzeFrameChangeTime(m_DateFrame * dfrm)
{
  //m_DateFrame * dfrm = (m_DateFrame *)buf;
  RTC_TIME_Type RTCFullTime;
  /* Set current time for RTC */  
  // Current time is 06:45:00PM, 2011-03-25
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, dfrm->m_ucSecond);
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, dfrm->m_ucMinutes);
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, dfrm->m_ucHour);
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, dfrm->m_ucMonth);
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, dfrm->m_iYear);
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, dfrm->m_ucDayOfMonth);
  
  // Get and print current time
  
  //RTC_GetFullTime (LPC_RTC, &RTCFullTime);
#ifndef RELEASE_CODE
  //_DBG( "Current time set to: ");
  //_DBD((RTCFullTime.HOUR)); _DBG (":");
  //_DBD ((RTCFullTime.MIN)); _DBG (":");
  //_DBD ((RTCFullTime.SEC)); _DBG("  ");
  //_DBD ((RTCFullTime.DOM)); _DBG("/");
  //_DBD ((RTCFullTime.MONTH)); _DBG("/");
  //_DBD16 ((RTCFullTime.YEAR)); _DBG_("");
#endif
        
  return 0;
}

unsigned char AnalyzeFrameReadDateTime(RTC_TIME_Type * rtcful)
{
  // Get and print current time
  
  RTC_GetFullTime (LPC_RTC, rtcful);
#ifndef RELEASE_CODE
  //_DBG( "Current time set to: ");
  //_DBD((RTCFullTime.HOUR)); _DBG (":");
  //_DBD ((RTCFullTime.MIN)); _DBG (":");
  //_DBD ((RTCFullTime.SEC)); _DBG("  ");
  //_DBD ((RTCFullTime.DOM)); _DBG("/");
  //_DBD ((RTCFullTime.MONTH)); _DBG("/");
  //_DBD16 ((RTCFullTime.YEAR)); _DBG_("");
#endif        
        
  return 0;  
}