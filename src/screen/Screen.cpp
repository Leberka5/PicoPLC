#include <utility>

#include "include/defines.h"
#include "include/screen/Screen.h"


/********************************************************************
 * Screen base class methods
 ********************************************************************/
Screen::Screen(Adafruit_ST7789& lcd, std::string title) :
    lcd(lcd),
    title(std::move(title)),
    colorBg(COLOR_BLACK) { }

uint16_t Screen::centerText(uint16_t numChars)
{
    return (SCREEN_WIDTH / 2) - (numChars * FONT_WIDTH_2 / 2);
}


