/*
  ISOWeekNumber.cpp

  MIT License
  Copyright (c) 2023 hdrlux  
*/

#include <Arduino.h>  // for all the 'standard' Arduino stuff
#include "ISOWeekNumber.h"

byte ISOdayOfWeek(long days) {  // TimeLib.h deviates from ISO8601 with Sunday = 1
  // ONLY valid for Epoch year 1970
  return (((days + 3) % DAYS_PER_WEEK) + 1);  // Mon=1..Sun=7
}

byte ISOWeekNumber(time_t t) {  // uses TimeLib.h - valid until 2099
  // weeknumber ONLY valid for Mon=1..Sun=7 datelibrary
  // calculations valid for ANY Epoch year, only relative dates
  long NowDay = elapsedDays(t);                       // days_since_epoch: now
  long CloseThu = NowDay - ISOdayOfWeek(NowDay) + 4;  // days_since_epoch: closest Thursday from now
  int ISOYear = year(CloseThu * SECS_PER_DAY);        // Year for closest Thursday, may be different in first or last week
  tmElements_t tm;
  tm.Hour = 0;
  tm.Minute = 0;
  tm.Second = 0;
  tm.Day = 4;    // 4th day according ISO8601
  tm.Month = 1;  // Jan
  tm.Year = CalendarYrToTm(ISOYear);
  time_t JanFour = makeTime(tm);                                    // date: Year-Jan-4
  long JanFourDay = elapsedDays(JanFour);                           // days_since_epoch: Year-Jan-4
  int ISOYearDay = NowDay - JanFourDay + ISOdayOfWeek(JanFourDay);  // Day number in the ISOYear, range [1..371]
  return (((ISOYearDay - 1) / 7) + 1);                              // Convert Day number in the ISOYear to ISOWeek number, range [1..53]
}

//End