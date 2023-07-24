/*
  LED_96x8_matrix.cpp

  MIT License
  Copyright (c) 2023 hdrlux
*/

#include <Arduino.h>     // for all the 'standard' Arduino stuff
#include <MD_MAX72xx.h>  // https://github.com/MajicDesigns/MD_MAX72XX
#include "LED_96x8_matrix.h"
#include "Font_data.h"
#include "RTC_com.h"  // for Sync_Err

// Define the number of devices we have in the chain and the hardware interface
#define MAX_DEVICES 12                     // total amount of 8x8 LED segments
#define LEFT_DEVICES 5                     // amount of 8x8 LED segments in left zone
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  // check if you get strange pixel print
#define CLK_PIN 13                         // or SCK [is also onboard LED on Nano]
#define DATA_PIN 11                        // or MOSI
#define CS_PIN 10                          // or SS
#define INIT_BRIGHT 0                      // inital intensity

#define DIP1 6  // Scroll DIP switch
#define DIP2 7  // Bright DIP switch

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Text parameters
#define CHAR_SPACING 1  // pixels between characters

void Matrix_setup() {
  mx.begin();
  mx.setFont(fontClock);  // custom font in Font_data.h
  delay(100);             // wait init
  Bright_time_zone(INIT_BRIGHT);
  Bright_date_zone(INIT_BRIGHT);
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
}

void Bright_date_zone(byte value) {  // set range 0..15
  mx.control(0, MAX_DEVICES - LEFT_DEVICES - 1, MD_MAX72XX::INTENSITY, value);
}

void Bright_time_zone(byte value) {  // set range 0..15
  mx.control(MAX_DEVICES - LEFT_DEVICES, MAX_DEVICES - 1, MD_MAX72XX::INTENSITY, value);
}

void DIP_Bright() {
  if (digitalRead(DIP2) == LOW) {
    Bright_time_zone(3);
    Bright_date_zone(3);
  } else {
    Bright_time_zone(INIT_BRIGHT);
    Bright_date_zone(INIT_BRIGHT);
  }
}

bool DIP_Scroll() {
  if (digitalRead(DIP1) == LOW) {
    return true;
  }
  return false;
}

void Print_date_zone(char *my_text) {
  char TextBuf[13];  // string buffer, max n-1 char
  char err_char;
  if (SyncErr) {
    err_char = '!';  // sync warning on display
  } else {
    err_char = ' ';  // space
  }
  snprintf(TextBuf, sizeof(TextBuf), "%c%s", err_char, my_text);  // add char before text, 'between' zones
  printText(0, MAX_DEVICES - LEFT_DEVICES - 1, TextBuf);          // right zone, from input side
}

void Print_time_zone(char *my_text) {  // assuming max 8 chars
  DIP_Bright();
  printText(MAX_DEVICES - LEFT_DEVICES, MAX_DEVICES - 1, my_text);  // left zone
}

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
/***** HowTo **********************************************
  Simple text printing, uses minimal memory
  Print the text chars to the LED matrix modules specified.
  Message area is padded with blank columns after printing.
**********************************************************/
{
  uint8_t state = 0;
  uint8_t curLen;
  uint16_t showLen;
  uint8_t cBuf[8];
  int16_t col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do  // finite state machine to print the characters in the space available
  {
    switch (state) {
      case 0:  // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0') {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying

      case 1:  // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen) {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2:  // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:  // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;  // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

//End