/*
  GPS_RTC_Clock.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>  // for all the 'standard' Arduino stuff
#include "GPS_RTC_Clock.h"
#include "RTC_com.h"
#include "GPS_com.h"
#include "LocalDateTime.h"

bool NewSec = false;
bool NewMin = false;
bool NewHour = false;
time_t Loc_t = 0;
byte last_min = -1;
byte last_5min = -1;
byte last_hour = -1;
bool ResetMCU = true;

void GPS_RTC_Clock_setup() {
  RTC_LED_setup();
  RTC_setup();
  GPS_setup();
  TZ_setup();
}

void GPS_RTC_Clock_loop() {
  GPS_loop();
  RTC_loop();
}

void Sec_Flip(time_t t) {  // called from RTC_com
  Loc_t = TZ_Sec(t);       // convert UTC to Local Time
  NewSec = true;           // set "Ready to print"
  if (ResetMCU) {          // set init time
    ResetMCU = false;
    last_5min = minute(Loc_t) / 5;
    last_hour = hour(Loc_t);
  }
  if (last_min != minute(Loc_t)) {  // Check new minute
    MinFlip();
  }
}

void MinFlip() {
  last_min = minute(Loc_t);
  if (last_5min != last_min / 5) {  // Check new 5-min
    Min5Flip();
  }
  if (last_hour != hour(Loc_t)) {  // Check new hour
    HourFlip();
  }
  NewMin = true;  // set "Ready to print"
}

void Min5Flip() {
  last_5min = last_min / 5;
  GPS_ON();  // try sync RTC-GPS
}

void HourFlip() {  // do something on the flip of the RTC hour [UTC time]
  last_hour = hour(Loc_t);
  NewHour = true;  // set "Ready to print"
}

//End