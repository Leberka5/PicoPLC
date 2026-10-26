#include <utility>

#include "include/defines.h"
#include "include/screen/Screen.h"


/********************************************************************
 * Screen base class methods
 ********************************************************************/
Screen::Screen(Adafruit_ST7789& lcd, node_ptr<Screen*>& nodeCurr,  std::string title) :
    lcd(lcd),
    nodeCurr(nodeCurr),
    title(std::move(title)),
    currSelEntry(0),
    colorBg(COLOR_BLACK) { }

Screen::Screen(Adafruit_ST7789& lcd, node_ptr<Screen*>& nodeCurr, std::string title, std::vector<std::string> entries, bool hasBackBtn) :
    lcd(lcd),
    nodeCurr(nodeCurr),
    title(std::move(title)),
    entries(std::move(entries)),
    currSelEntry(0),
    colorBg(COLOR_BLACK)
{
    if (hasBackBtn)
        this->entries.emplace_back("Back");
}

uint16_t Screen::centerText(uint16_t numChars)
{
    return (SCREEN_WIDTH / 2) - (numChars * FONT_WIDTH_2 / 2);
}


