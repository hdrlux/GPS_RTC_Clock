/*
  ISOWeekNumber.h

  MIT License
  Copyright (c) 2023 hdrlux
  
  function:
  - provides weeknumber from Unix time, according ISO8601
*/

#ifndef ISOWeekNumber_H
#define ISOWeekNumber_H

#include <TimeLib.h>    // https://github.com/PaulStoffregen/Time valid until year 2099, no 2038 bug !

// ISO8601 compliant, 1 = Monday
byte ISOdayOfWeek(long days);

// ISO8601 compliant, ISOWeek number, range [1..53]
byte ISOWeekNumber(time_t t);

#endif  // ISOWeekNumber_H