/*
  RTC_com.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>    // for all the 'standard' Arduino stuff
#include <DS3232RTC.h>  // https://github.com/JChristensen/DS3232RTC
#include <TimeLib.h>    // https://github.com/PaulStoffregen/Time [valid until year 2099, no 2038 bug]
#include "RTC_com.h"

// add the static modifier to limit visibility of these variables to just this file
static int ledPin_PPS = A6;      // Pin for RTC PseudoPPS LED [analog PIN used as digital PIN]
static int ledPin_Sync = A7;     // Pin for valid RTC Sync
static byte RTC_1HZ_PIN = 3;     // Pin 3 = NANO INT1, RTC provides a 1Hz interrupt signal on this Pin
static time_t last_sync_t = 0;   // last sync
static byte sync_err_hours = 4;  // set to 4 for DS3231M, may be set to 8 for DS3231SN. Warning if no GPS sync after n hours, possible 0.1 sec error.
bool SyncErr = true;

#include <DS3232RTC.h>
DS3232RTC myRTC;
volatile bool RTC_sec = false;   // flag for PseudoPPS
static unsigned long RTCMillis;  // blinking timer

void rtc_interrupt() {
  RTCMillis = millis();
  RTC_sec = true;
}

void RTC_LED_setup() {
  pinMode(ledPin_PPS, OUTPUT);
  pinMode(ledPin_Sync, OUTPUT);
  digitalWrite(ledPin_PPS, LOW);   // LED off
  digitalWrite(ledPin_Sync, LOW);  // LED off
}

void RTC_setup() {
  pinMode(RTC_1HZ_PIN, INPUT_PULLUP);                                           // enable pullup on interrupt pin (RTC SQW pin is open drain)
  attachInterrupt(digitalPinToInterrupt(RTC_1HZ_PIN), rtc_interrupt, FALLING);  // HIGH 500ms after start of second
  myRTC.begin();
  myRTC.squareWave(DS3232RTC::SQWAVE_1_HZ);  // 1 Hz square wave
  RTC_sec = false;
}

void RTC_loop() {
  if (RTC_sec) {                    // do after RTC PseudoPPS interrupt, without interrupt NO time/date to display
    RTC_sec = false;                // clear flag
    digitalWrite(ledPin_PPS, LOW);  // LED off
    time_t now_t = myRTC.get();
    if ((last_sync_t + (sync_err_hours * 3600)) < now_t) {
      SyncErr = true;
      digitalWrite(ledPin_Sync, LOW);  // LED off
    }
    Sec_Flip(now_t);  // pass current datetime from RTC in UTC
  }
  if (millis() > (RTCMillis + 100)) {  // do 100ms after PseudoPPS interrupt
    digitalWrite(ledPin_PPS, HIGH);    // LED on
  }
}

void SetRTC(time_t t) {
  myRTC.set(t + 1);  // the GPS NMEA output is 1 second behind at PPS !!
  last_sync_t = t;
  SyncErr = false;
  digitalWrite(ledPin_Sync, HIGH);   // LED on
  Serial.println("RTC set by GPS");  // debug
}

//End