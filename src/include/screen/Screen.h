#ifndef PICOPLC_SCREEN_H
#define PICOPLC_SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "../defines.h"


// just some mappings for options of Adafruit_ST7789
enum LcdNumerics
{
    FONT_6x8 = 1,
    FONT_12x16 = 2, // currently in usee
    FONT_18x24 = 3,
    ROTATION_UP = 2
};

// metrics for a 240x240 lcd display
enum GuiNumerics
{
    SCREEN_WIDTH = 240,
    SCREEN_HEIGHT = 240,
    FONT_HEIGHT_1 = 8,      // name refers to val LcdNumerics::FONT_XXX
    FONT_HEIGHT_2 = 16,
    FONT_HEIGHT_3 = 24,
    FONT_WIDTH_1 = 6,       // name refers to val LcdNumerics::FONT_XXX
    FONT_WIDTH_2 = 12,
    FONT_WIDTH_3 = 18,
    PADDING_TITLE_X = 30,   // delta x for title
    PADDING_TITLE_Y = 6,    // delta y for title
    PADDING_MENU_X = 4,
    PADDING_MENU_Y = 4,
    TITLE_HEIGHT = FONT_HEIGHT_3 + 2*PADDING_TITLE_Y,
    MENU_SELECTION_HEIGHT = FONT_HEIGHT_2 + 2*PADDING_MENU_Y    // height for encoder sel in GUI
};


/********************************************************************
* base class for ScreenMenu, ScreenError
 * override draw() in oder to use it
 ********************************************************************/
class Screen
{
public:
    Screen(Adafruit_ST7789& lcd, std::string title);
    virtual ~Screen() = default; // virtual in order to call derived class's destructor when deleted
    virtual void draw() = 0;
protected:
    uint16_t centerText(uint16_t numChars);
    Adafruit_ST7789& lcd;
    std::string title;
    uint16_t colorBg;
};


#endif //PICOPLC_SCREEN_H
