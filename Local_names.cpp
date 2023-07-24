/*
  Local_names.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>  // for all the 'standard' Arduino stuff
#include "Local_names.h"

#define dt_MAX_STRING_LEN 9
#define dt_SHORT_STR_LEN 3

static char buffer_m[dt_MAX_STRING_LEN + 1];
static char buffer_d[dt_MAX_STRING_LEN + 1];

//const char monthShortNames_P[] PROGMEM = "ErrJanFebMarAprMayJunJulAugSepOctNovDec";  // Translate to local Language
//const char dayShortNames_P[] PROGMEM = "ErrSunMonTueWedThuFriSat";                   // Translate to local Language
const char monthShortNames_Loc[] PROGMEM = "ErrJanFebMrtAprMeiJunJulAugSepOktNovDec";  // Dutch
const char dayShortNames_Loc[] PROGMEM = "ErrZonMaaDinWoeDonVryZat";                   // Dutch

char* monthShortStrLoc(uint8_t month) {
  for (int i = 0; i < dt_SHORT_STR_LEN; i++)
    buffer_m[i] = pgm_read_byte(&(monthShortNames_Loc[i + (month * dt_SHORT_STR_LEN)]));
  buffer_m[dt_SHORT_STR_LEN] = 0;
  return buffer_m;
}

char* dayShortStrLoc(uint8_t day) {
  uint8_t index = day * dt_SHORT_STR_LEN;
  for (int i = 0; i < dt_SHORT_STR_LEN; i++)
    buffer_d[i] = pgm_read_byte(&(dayShortNames_Loc[index + i]));
  buffer_d[dt_SHORT_STR_LEN] = 0;
  return buffer_d;
}

//End