/*
  GPS_RTC_Clock.h

  MIT License
  Copyright (c) 2023 hdrlux

  main local Lib with Clock control, creating 'long term ±0.1 sec accurate Arduino clock'

  tested hardware:
  -Arduino Nano 5V ATmega328P 
  -see local *.h files for other hardware

  main functions:
  - Sync RTC with GPS in UTC using interrupt
  - LED blinking in sync with GPS PPS, to check accuracy
  - LED ON if RTC synced in last hours
  - Convert to Local Date/Time
  - Setting flag for printing new time to display, using interrupt
*/

/***** Notice *******************************************
  !! GPS & USB use the same serial port !!              
  you will have to disconnect the Arduino RX pin 0 from  
  the GPS TX pin, to upload a new sketch over USB       
  ->remove GPS PCB or Nano PCB from main PCB/breadboard 
********************************************************/

#ifndef GPS_RTC_Clock_H
#define GPS_RTC_Clock_H

#include "ISOWeekNumber.h"  // pass to main ino
#include "Local_names.h"    // pass to main ino
#include "RTC_com.h"

extern bool NewSec;   // pass to main ino = second ready to print
extern bool NewMin;   // pass to main ino = minute ready to print
extern bool NewHour;  // pass to main ino = Hour ready to print
extern time_t Loc_t;  // pass to main ino = Local timestamp

// call from main ino, GPS_RTC_Clock.h
void GPS_RTC_Clock_setup();

// call from main ino, GPS_RTC_Clock.h
void GPS_RTC_Clock_loop();

// process sec from RTC
void Sec_Flip(time_t t);

// do something on reboot & the flip of the RTC minute [timezone independent]
void MinFlip();

// do something every 5 minutes
void Min5Flip();

// do something every hour, on the hour
void HourFlip();

#endif  // GPS_RTC_Clock_H