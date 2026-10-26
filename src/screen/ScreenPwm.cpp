#include "../include/screen/ScreenPwm.h"

#include "include/Gui.h"


ScreenPwm::ScreenPwm(Adafruit_ST7789 &lcd, node_ptr<Screen*>& nodeCurr, const std::string &menuTitle, std::vector<std::string> entries) :
    Screen(lcd, nodeCurr, menuTitle, entries)
{

}

ScreenPwm& ScreenPwm::operator=(const ScreenPwm& other)
{
    if (this == &other)
        return *this;
    // copy ALL members
    this->lcd = other.lcd;
    this->title = other.title;
    this->colorBg = other.colorBg;

    return *this;
}

void ScreenPwm::draw()
{
    currCursorY = PADDING_TITLE_Y;
    // draw menu title
    lcd.setTextColor(COLOR_GREEN_2);
    lcd.setTextSize(FONT_12x16);
    lcd.setCursor(PADDING_TITLE_X, currCursorY);
    lcd.write(title.c_str());
    // draw menu entries
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(FONT_12x16);
    currCursorY = TITLE_HEIGHT + PADDING_MENU_Y;
    lcd.setCursor(PADDING_MENU_X, currCursorY);
    for (std::string currMenuEntry : entries)
    {
        // write menu entry
        lcd.write(currMenuEntry.c_str());
        // update cursor for next menu entry
        currCursorY += MENU_SELECTION_HEIGHT;
        lcd.setCursor(PADDING_MENU_X, currCursorY + PADDING_MENU_Y);
    }
    drawSelUpdate(INIT);
}

void ScreenPwm::drawSelUpdate(MenuActions actionMenuSel)
{
    // clear out old selection
    uint8_t oldCursorY = currSelEntry * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, oldCursorY, colorBg);
        lcd.drawPixel(x, oldCursorY + MENU_SELECTION_HEIGHT, colorBg);
    }

    // adjust new menu selection
    if (actionMenuSel == UP)
        currSelEntry--;
    else if (actionMenuSel == DOWN)
        currSelEntry++;
    if (currSelEntry < 0)
        currSelEntry = entries.size() - 1;
    else if (currSelEntry >= entries.size())
        currSelEntry = 0;

    // draw new selection
    uint8_t currCursorY = currSelEntry * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, currCursorY, COLOR_GREEN);
        lcd.drawPixel(x, currCursorY + MENU_SELECTION_HEIGHT, COLOR_GREEN);
    }
}

void ScreenPwm::buttonPress()
{
    
}