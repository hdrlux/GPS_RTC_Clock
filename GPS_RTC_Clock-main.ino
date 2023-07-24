/* GPS_RTC_Clock

  MIT License
  Copyright (c) 2023 hdrlux

  https://www.instructables.com/01-Sec-Accurate-RTC-GPS-Wall-Clock-Arduino-96x8-LE/
  https://github.com/hdrlux/GPS_RTC_Clock
*/

#include "GPS_RTC_Clock.h"
#include "LED_96x8_matrix.h"
char TempBuf[5] = "99.9";  // demo value
char HumiBuf[3] = "99";    // demo value


void setup() {                         // the setup function runs once when you press reset or power the board
  GPS_RTC_Clock_setup();               // first in setup
  Matrix_setup();                      // LED display
  Serial.begin(9600);                  // = 9600, must be same as GPS for debug
  Serial.println();                    // flush serial
  Serial.println("-Arduino Reboot-");  // debug
}

void loop() {            // the loop function runs over and over again forever
  GPS_RTC_Clock_loop();  // first in loop
  PrintSec();
  PrintHour();
}

void PrintSec() {    // print time if new second
  if (NewSec) {      //
    NewSec = false;  // remove flag,  do only once every sec
    char startmarker = '<';
    char endmarker = '>';
    Serial.print(startmarker);  // for remote display via RS485
    Serial.print(Loc_t);        // for remote display via RS485
    Serial.println(endmarker);  // for remote display via RS485

    char TimeBuf[9];                                      // time string buffer, max n-1  char
    snprintf(TimeBuf, sizeof(TimeBuf), "%.2u:%.2u:%.2u",  // https://cplusplus.com/reference/cstdio/printf/
             hour(Loc_t), minute(Loc_t), second(Loc_t));  // time 24h format
    Serial.print("Time: ");                               // debug
    Serial.println(TimeBuf);                              // debug
    Print_time_zone(TimeBuf);                             // print to LED Matrix
    PrintMin();
  }
}

void PrintMin() {
  char TextBuf[12];     // date string buffer, max n-1  char
  if (!DIP_Scroll()) {  // print date only or 'scrolling' text
    snprintf(TextBuf, sizeof(TextBuf), "%s %.2u %s",
             dayShortStrLoc(weekday(Loc_t)), day(Loc_t), monthShortStrLoc(month(Loc_t)));  // weekday, day, month
  } else {                                                                                 // print 'scrolling'
    /*** HowTo ******************************************************************
    print different info every few seconds, because of limited display space
    define the amount of different text lines, must be at least 1
    define the amount of seconds each textline is displayed, must be at least 1
    define each text line, this may be duplicates of other text lines
    *****************************************************************************/
    byte text_lines = 3;                                         // amount of different text lines printed
    byte text_timer = 5;                                         // amount of seconds to next text
    byte text_counter(((Loc_t / text_timer) % text_lines) + 1);  // range = 1..n
    if (text_counter == 1) {
      snprintf(TextBuf, sizeof(TextBuf), "%.2u%s%u",
               day(Loc_t), monthShortStrLoc(month(Loc_t)), year(Loc_t));  // day, month, year
    } else if (text_counter == 2) {
      snprintf(TextBuf, sizeof(TextBuf), "%s W%.2u",
               dayShortStrLoc(weekday(Loc_t)), ISOWeekNumber(Loc_t));  // weekday, week
    } else if (text_counter == 3) {
      snprintf(TextBuf, sizeof(TextBuf), "%s*C %s%%", TempBuf, HumiBuf);  // * = ° degrees char in fontClock
    } else {
      snprintf(TextBuf, sizeof(TextBuf), " Err ");
    }
  }
  Print_date_zone(TextBuf);  // print to LED Matrix

  if (NewMin) {        // print date if new minute
    NewMin = false;    // remove flag, do only once every min
    char DateBuf[21];  // date string buffer, max n-1  char
    // long date version
    snprintf(DateBuf, sizeof(DateBuf), "%s %.2u-%s-%u W%.2u",
             dayShortStrLoc(weekday(Loc_t)), day(Loc_t), monthShortStrLoc(month(Loc_t)), year(Loc_t), ISOWeekNumber(Loc_t));
    Serial.print("Date long: ");  // debug
    Serial.println(DateBuf);      // debug
  }
}

void PrintHour() {                               // do if new hour
  if (NewHour) {                                 //
    NewHour = false;                             // remove flag, do only once every hour
    Serial.println("bring out the Cuckoo ;-)");  // debug
  }
}

//End
