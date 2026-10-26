#include "../include/screen/ScreenError.h"


/********************************************************************
 * ScreenError methods
 ********************************************************************/
ScreenError::ScreenError(Adafruit_ST7789& lcd) :
    Screen(lcd, "Error:") { }

void ScreenError::setErrorMsg(const std::string& errorMsg)
{
    errorLog.emplace_back(errorMsg);
    this->errorMsg = errorMsg;
}

void ScreenError::draw()
{
    // 12px char len
    lcd.setTextColor(COLOR_RED);
    lcd.setCursor(centerText(title.length()), 50);
    lcd.write(title.c_str());
    lcd.setTextColor(COLOR_YELLOW);
    lcd.setCursor(0, 100);
    lcd.write(errorMsg.c_str());
}

void ScreenError::drawSelUpdate(MenuActions actionMenuSel)
{

}