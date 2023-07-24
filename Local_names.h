/*
  Local_names.h

  MIT License
  Copyright (c) 2023 hdrlux
  
  Convert to short local weekday & month names
  Workaround because DateStrings.cpp in TimeLib is english only
*/

#ifndef Local_names_H
#define Local_names_H

// Local short month
char* monthShortStrLoc(uint8_t month);

// Local short weekday
char* dayShortStrLoc(uint8_t day);

#endif  // Local_names_H