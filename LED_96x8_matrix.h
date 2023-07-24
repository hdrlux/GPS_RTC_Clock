/*
  LED_96x8_matrix.h

  MIT License
  Copyright (c) 2023 hdrlux
  
  all LED Display functions, specific hardware:
  - MAX7219 with 8x8 LED segment 32x32mm
  - unit FC16 = 4 segments on one PCB
  - 3 units in series = chain of 12 segments, making a 96x8 display 
  - if NOT wired correct, ALL LED's are ON in HIGH brightness, using 9W power -> use 10W 5V/2A power source for project safety
  - normal operation uses 0,5W for LED display + 0,5W for control board
*/

#ifndef LED_96x8_matrix_H
#define LED_96x8_matrix_H

// main display setup
void Matrix_setup();

// intensity right
void Bright_date_zone(byte value);

// intensity left
void Bright_time_zone(byte value);

// Check DIP switch for brightness control
void DIP_Bright();

// Check DIP switch for text scrolling
bool DIP_Scroll();

// print right
void Print_date_zone(char *my_text);

// print left
void Print_time_zone(char *my_text);

// print chars to specific segment range, NO animations
void printText(uint8_t modStart, uint8_t modEnd, char *pMsg);

#endif  // LED_96x8_matrix_H