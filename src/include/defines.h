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

// used to change the currently selected menu entry (via encoder)
enum MenuActions
{
    INIT,
    DOWN,
    UP,
};

// 16 bit colors for Adafruit_ST7789
enum Colors
{
    COLOR_WHITE = 0xFFFF,
    COLOR_BLACK = 0x0000,
    COLOR_GREEN = 0x0f00,
    COLOR_GREEN_2 = 0x07E0,
    COLOR_RED = 0xF800,
    COLOR_BLUE = 0x001F,
    COLOR_BLUEGREY = 0x22ED,
    COLOR_CYAN = 0x07FF,
    COLOR_MAGENTA = 0xF81F,
    COLOR_YELLOW = 0xFFE0,
    COLOR_ORANGE = 0xFC00
};


using INTERRUPT = void;


#endif //DEFINES_H
