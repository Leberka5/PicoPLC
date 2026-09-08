#include "include/defines.h"
#include "include/screen/screen.h"



/********************************************************************
 * Screen base class methods
 ********************************************************************/
Screen::Screen(Adafruit_ST7789& lcd) :
    lcd(lcd),
    colorBg(COLOR_BLACK) { }

uint16_t Screen::centerText(uint16_t numChars) { return (SCREEN_WIDTH / 2) - (numChars * FONT_WIDTH_2 / 2); }


/********************************************************************
 * ScreenError methods
 ********************************************************************/
ScreenError::ScreenError(Adafruit_ST7789& lcd) :
    Screen(lcd) { }

void ScreenError::setErrorMsg(std::string errorMsg)
{
    errorLog.emplace_back(errorMsg);
    this->errorMsg = errorMsg;
}

void ScreenError::draw()
{
    // 12px char len
    std::string strHead = "Error:";
    lcd.setTextColor(COLOR_RED);
    lcd.setCursor(centerText(strHead.length()), 50);
    lcd.write(strHead.c_str());
    lcd.setTextColor(COLOR_YELLOW);
    lcd.setCursor(0, 120);
    lcd.write(errorMsg.c_str());
}


/********************************************************************
 * methods of class GuiMenu
 ********************************************************************/
ScreenMenu::ScreenMenu(Adafruit_ST7789& lcd, const std::string& menuTitle, const std::vector<std::string>& menuEntries, bool hasBackBtn) :
    Screen(lcd),
    menuTitle(menuTitle),
    submenuNames(menuEntries),
    currSelection(0)
{
    if (hasBackBtn)
        this->submenuNames.emplace_back("Back");
}

ScreenMenu& ScreenMenu::operator=(const ScreenMenu& other)
{
    if (this == &other)
        return *this;
    // copy ALL member of GuiMenu
    this->lcd = other.lcd;
    this->menuTitle = other.menuTitle;
    this->currSelection = other.currSelection;
    this->colorBg = other.colorBg;

    return *this;
}

void ScreenMenu::draw()
{
    uint8_t currCursorY = PADDING_TITLE_Y;
    // draw menu title
    lcd.setTextColor(COLOR_GREEN_2);
    lcd.setTextSize(FONT_12x16);
    lcd.setCursor(PADDING_TITLE_X, currCursorY);
    lcd.write(menuTitle.c_str());
    // draw menu entries
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(FONT_12x16);
    currCursorY = TITLE_HEIGHT + PADDING_MENU_Y;
    lcd.setCursor(PADDING_MENU_X, currCursorY);
    for (std::string currMenuEntry : submenuNames)
    {
        // write menu entry
        lcd.write(currMenuEntry.c_str());
        // update cursor for next menu entry
        currCursorY += MENU_SELECTION_HEIGHT;
        lcd.setCursor(PADDING_MENU_X, currCursorY + PADDING_MENU_Y);
    }
    drawSelUpdate(INIT);

}

void ScreenMenu::drawSelUpdate(MenuActions actionMenuSel)
{
    // clear out old selection
    uint8_t oldCursorY = currSelection * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, oldCursorY, colorBg);
        lcd.drawPixel(x, oldCursorY + MENU_SELECTION_HEIGHT, colorBg);
    }

    // adjust new menu selection
    if (actionMenuSel == UP)
        currSelection--;
    else if (actionMenuSel == DOWN)
        currSelection++;
    if (currSelection < 0)
        currSelection = submenuNames.size() - 1;
    else if (currSelection >= submenuNames.size())
        currSelection = 0;

    // draw new selection
    uint8_t currCursorY = currSelection * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, currCursorY, COLOR_GREEN);
        lcd.drawPixel(x, currCursorY + MENU_SELECTION_HEIGHT, COLOR_GREEN);
    }
}

uint16_t ScreenMenu::getMenuSelection() { return currSelection; }

uint16_t ScreenMenu::getNumSubMenus() { return submenuNames.size(); }

std::string ScreenMenu::getSubmenuName(uint16_t index)
{
    return submenuNames.at(index);
}
