/*
  GPS_com.h

  MIT License
  Copyright (c) 2023 hdrlux

  all GPS time functions, tested hardware:
  - Ublox Neo M8N
  - External L1-antenna
  - set Pin: PPS Interrupt in file GPS_com.cpp [default: 2 = NANO INT0]
*/

/***** Notice ********************************************
  Edit file \Arduino\libraries\NeoGPS\src\NMEAGPS_cfg.h
  UnComment line //#define NMEAGPS_PARSE_ZDA
  only process the NMEA sentences GGA, RMC en ZDA
*********************************************************/

/****** Notice *******************************************
  Edit file \Arduino\libraries\NeoGPS\src\NeoTime.h
  change both instances of the const 'DAYS_PER_WEEK'
  to something else, they conflict with TimeLib.h
*********************************************************/

#ifndef GPS_com_H
#define GPS_com_H

// handle interrupt
void gps_interrupt();

// setup hardware & interrupt from PseudoPPS
void GPS_setup();

// main GPS loop, GPS_com
void GPS_loop();

// do something on the flip of the GPS second
void GPS_PPS();

// get time & amount of satts in FIX
void GPS_read_seconds();

// turn GPS on
void GPS_ON();

// no hardware OFF, just ignore
void GPS_OFF();

#endif  // GPS_com_H