#include "../include/screen/ScreenPwm.h"


ScreenPwm::ScreenPwm(Adafruit_ST7789 &lcd, const std::string &menuTitle) :
    Screen(lcd, menuTitle)
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
    lcd.setTextColor(COLOR_GREEN_2);
    lcd.setTextSize(FONT_12x16);
    lcd.setCursor(20, 20);
    for (uint16_t x = 0; x < lcd.width(); x++)
    {
        lcd.drawPixel(x, x, COLOR_GREEN_2);
    }
}

void ScreenPwm::drawSelUpdate(MenuActions actionMenuSel)
{

}
