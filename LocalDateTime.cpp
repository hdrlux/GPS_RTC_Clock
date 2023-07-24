/*
  LocalDateTime.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>  // for all the 'standard' Arduino stuff
#include <Timezone_Generic.h>  // https://github.com/khoih-prog/Timezone_Generic [only this one, DO NOT install all dependencies !!]
#include "LocalDateTime.h"

/*** HowTo ****************************************************************
  TimeChangeRule myRule = {abbrev, week, dow, month, hour, offset};
  abbrev: is a character string abbreviation for the time zone, 
   it must be no longer than five characters.
  week: is the week of the month that the rule starts.
  dow: is the day of the week that the rule starts.
  hour: is the hour in local time that the rule starts (0-23).
  offset: is the UTC offset in minutes for the time zone being defined.

  For convenience, the following symbolic names can be used:
  week: First, Second, Third, Fourth, Last
  dow: Sun, Mon, Tue, Wed, Thu, Fri, Sat
  month: Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
**************************************************************************/

/*** Instruction ******************************************************************
  Select ONE set of TimeChangeRules below, also for non DST zones
  Selection NOT possible during runtime, only at compiletime
  The timezones are defined by the offset, in minutes, from UTC. 
  This means the start of a new minute, and the seconds, are timezone independent !
**********************************************************************************/

// Eastern US time zone [Ottawa, New York, Miami, Nassau]
//TimeChangeRule mySTD = { "EST", First, Sun, Nov, 2, -300 };  // change to UTC - 5 hours
//TimeChangeRule myDST = { "EDT", Second, Sun, Mar, 2, -240 }; // change to UTC - 4 hours

// Western European Time Zone [London = GMT/BST, Dublin = GMT/IST, Lisboa = WET/WEST, Canarias]
//TimeChangeRule mySTD = { "WET ", Last, Sun, Oct, 2, 0 };  // change to UTC
//TimeChangeRule myDST = { "WEST", Last, Sun, Mar, 1, 60 }; // change to UTC + 1 hours

// Central European Time Zone, directive 2000/84/EC [Bruxelles, Paris, Madrid, Roma, Beograd, Warszawa, Stockholm, Berlin]
// All 3 European timezones change at the same time: 01:00 UTC
TimeChangeRule mySTD = { "CET ", Last, Sun, Oct, 3, 60 };   // change to UTC + 1 hours
TimeChangeRule myDST = { "CEST", Last, Sun, Mar, 2, 120 };  // change to UTC + 2 hours

// Eastern European Time Zone [Helsinki, Rīga, Kyiv, Αθήνα]
//TimeChangeRule mySTD = { "EET ", Last, Sun, Oct, 4, 120 }; // change to UTC + 2 hours
//TimeChangeRule myDST = { "EEST", Last, Sun, Mar, 3, 180 }; // change to UTC + 3 hours

// Australia Eastern Time Zone [Canberra, Brisbane, Sydney, Melbourne, Hobart]
//TimeChangeRule mySTD = { "AEST", First, Sun, Apr, 3, 600 }; // change to UTC + 10 hours
//TimeChangeRule myDST = { "AEDT", First, Sun, Oct, 2, 660 }; // change to UTC + 11 hours

// Москва Standard Time (MSK, does not observe DST)
//TimeChangeRule mySTD = {"MSK ", Last, Sun, Oct, 1, 180}; // allways UTC + 3 hours
//TimeChangeRule myDST = {"MSK ", Last, Sun, Mar, 1, 180}; // allways UTC + 3 hours

// Universal Time Coordinated [UTC]
//TimeChangeRule mySTD = {"UTC ", Last, Sun, Oct, 1, 0};
//TimeChangeRule myDST = {"UTC ", Last, Sun, Mar, 1, 0};

Timezone* myTZ;

void TZ_setup() {
  myTZ = new Timezone(myDST, mySTD);
}

time_t TZ_Sec(time_t t) {  // convert UTC to Local-Time
  TimeChangeRule* tcr;   // pointer to the time change rule, use to get TZ abbrev 
  return (myTZ->toLocal(t, &tcr));
}

//End