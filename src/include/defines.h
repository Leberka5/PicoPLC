/*
 * This file is being included very often. Don't bloat it!
 *
 * commonly used abbreviations in comments and symbol names:
 *      - sel:      selection
 *      - curr:     current
 *      - num:      number
 *      - vec:      vector
 *      - elem:     element
 *      - iX:       index var used in for loops (e.g. iElem)
 *      - ret:      return
 *      - enc:      rotary encoder (360 degree digital potentioter)
 */

#ifndef DEFINES_H
#define DEFINES_H

// frequently used includes which shall be globally accessible
#include <Arduino.h>
#include <vector>
#include <string>
#include <queue>


enum pins
{
    LED = 25,
    SPI_CS = -1,            // SPI Chip Select (disabled)
    LCD_RST = 20,           // reset pin
    LCD_DC = 21,            // data / cmd pin
    ENCODER_A = 16,         // encoder output a
    ENCODER_B = 17,         // encoder output a
    ENCODER_BUTTON = 22     // encoder output a
};

using INTERRUPT = void;


#endif //DEFINES_H
