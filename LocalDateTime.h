/*
  LocalDateTime.h

  MIT License
  Copyright (c) 2023 hdrlux
  
  function:
  - Convert UTC timestamp to Local timestamp, including DST detection [=winter/summer time]

  usage:
  - uncomment ONE timezone in file LocalDateTime.cpp
*/

#ifndef LocalDateTime_H
#define LocalDateTime_H

#include <TimeLib.h>    // https://github.com/PaulStoffregen/Time valid until year 2099, no 2038 bug !

// main TZ setup, GPS_RTC_Clock
void TZ_setup();

// convert UTC to Local-Time
time_t TZ_Sec(time_t t);

#endif  // LocalDateTime_H