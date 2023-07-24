/*
  GPS_com.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>  // for all the 'standard' Arduino stuff
#include <NMEAGPS.h>  // https://github.com/SlashDevin/NeoGPS
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
#include "GPS_com.h"
#include "RTC_com.h"  // to set the RTC

// add the static modifier to limit visibility of these variables to just this file
static byte GPS_PPS_PIN = 2;  // Pin 2 = NANO INT0, GPS PPS interrupt signal on this Pin
static bool gps_on;           // flag for GPS ON/OFF
static int pulse_count = 0;
static time_t gps_seconds_t = 0;  // GPS time
static byte gps_sats = 0;
volatile bool GPS_sec = false;  // flag for GPS-PPS

// set interrupt flag
void gps_interrupt() {
  GPS_sec = true;
}

NMEAGPS gps;  // This parses the GPS characters
gps_fix fix;  // This holds on to the latest values
//#include <GPSport.h> // use only for soft-serial
#define gpsPort Serial
#define GPS_PORT_NAME "Serial"
#define DEBUG_PORT Serial

/***** Notice ********************************************
  Edit file \Arduino\libraries\NeoGPS\src\NMEAGPS_cfg.h
  UnComment line //#define NMEAGPS_PARSE_ZDA
  only process the NMEA sentences GGA, RMC en ZDA
*********************************************************/

/****** Notice *******************************************
  Edit file \Arduino\libraries\NeoGPS\src\NeoTime.h
  change both instances of the const name 'DAYS_PER_WEEK'
  to something else, they conflict with TimeLib.h
*********************************************************/

// Compile check processing for both RMC & ZDA message [both contain date & time]
// Less dependent on GPS chip config
#ifndef NMEAGPS_PARSE_RMC
#error You must define NMEAGPS_PARSE_RMC in NMEAGPS_cfg.h!
#endif
#ifndef NMEAGPS_PARSE_ZDA
#error You must define NMEAGPS_PARSE_ZDA in NMEAGPS_cfg.h!
#endif
#ifndef NMEAGPS_PARSE_GGA
#error You must define NMEAGPS_PARSE_GGA in NMEAGPS_cfg.h!
#endif
const long POSIX_Y2K_offset = 946684800;  // = 10957 days = 30 years, NeoTime (GPS) & TimeLib (RTC) library use different Epoch year, 2000 & 1970
const long offset_28y = 883612800;        // = 10227 days = 28 years, simple test for 2038 bug
const long offset_26w = 15724800;         // = 182 days = 26 weeks, simple test for timezone

void GPS_setup() {
  pinMode(GPS_PPS_PIN, INPUT_PULLUP);                                          // enable pullup on interrupt pin
  attachInterrupt(digitalPinToInterrupt(GPS_PPS_PIN), gps_interrupt, RISING);  // 100ms HIGH at start of second
  GPS_sec = false;
  gpsPort.begin(9600);  // set PC to same baudrate for debug messages
  GPS_ON();
}

void GPS_loop() {
  if (GPS_sec) {      // do after GPS PPS interrupt
    GPS_sec = false;  // clear flag
    GPS_PPS();
  }
  GPS_read_seconds();  // continue reading buffer
}

void GPS_PPS() {  // do something on the flip of the GPS second
  if (gps_on) {   // do only when needed
    pulse_count += 1;
    if (pulse_count > 2) {       // skip first PPS-Pulses, to make shure time is from satellite
      if (gps_seconds_t != 0) {  // do only if value is set
        // gps_seconds_t += offset_28y; // debug & testing only!
        // gps_seconds_t += offset_26w; // debug & testing only!
        SetRTC(gps_seconds_t);  // sync RTC with GPS
        GPS_OFF();
      }
    }
  }
}

void GPS_read_seconds() {
  while (gps.available(gpsPort)) {
    fix = gps.read();
    if (fix.valid.time && fix.valid.date) {
      gps_seconds_t = fix.dateTime + POSIX_Y2K_offset;  // convert for different epoch year
    }
    if (fix.valid.satellites) {
      gps_sats = fix.satellites;
    }
  }
}

void GPS_ON() {
  if (!gps_on) {  // only if NOT on
    gps_on = true;
    gps_seconds_t = 0;  // make shure GPS serial is alive before setting
    pulse_count = 0;
    Serial.println("GPS: ON");  // debug
  }
}

void GPS_OFF() {
  if (gps_on) {  // only if NOT off
    gps_on = false;
    Serial.println("GPS: OFF");  // debug
  }
}

//End