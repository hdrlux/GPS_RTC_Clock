/*
  RTC_com.h

  MIT License
  Copyright (c) 2023 hdrlux
  
  all RTC functions, tested hardware:
  - DS3231SN via I2C, has only 2-digit years, the Epoch is 2000-01-01. Valid until year 2099
  - set Pin: PseudoPPS Interrupt in file RTC_com.cpp [default: 3 = NANO INT1]
  - set Pin: PseudoPPS LED in file RTC_com.cpp [default: A3]
*/

#ifndef RTC_com_H
#define RTC_com_H

extern bool SyncErr;        // RTC not synced in last n hours
#include "GPS_RTC_Clock.h"  // for passing new second

// handle interrupt
void rtc_interrupt();

// setup the pin as output
void RTC_LED_setup();

// setup hardware & interrupt from PseudoPPS
void RTC_setup();

// main RTC loop, RTC_com
void RTC_loop();

// sync RTC from GPS
void SetRTC(time_t t);

#endif  // RTC_com_H