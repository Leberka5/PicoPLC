#include "../include/screen/ScreenMenu.h"
#include "../include/Gui.h"


/********************************************************************
 * methods of class GuiMenu
 ********************************************************************/
ScreenMenu::ScreenMenu(
    Adafruit_ST7789& lcd,
    node_ptr<Screen*>& nodeCurr,
    const std::string& menuTitle,
    const std::vector<std::string>& menuEntries,
    bool hasBackBtn) :
    Screen(lcd, nodeCurr, menuTitle, menuEntries, hasBackBtn)
{

}

ScreenMenu& ScreenMenu::operator=(const ScreenMenu& other)
{
    if (this == &other)
        return *this;
    // copy ALL member of GuiMenu
    this->lcd = other.lcd;
    this->title = other.title;
    this->currSelEntry = other.currSelEntry;
    this->colorBg = other.colorBg;

    return *this;
}

void ScreenMenu::draw()
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

void ScreenMenu::drawSelUpdate(MenuActions actionMenuSel)
{
    for (auto& entry : entries)
        Serial.println(entry.c_str());
    // clear out old selection
    int16_t oldCursorY = currSelEntry * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int16_t x = 0; x < SCREEN_WIDTH; ++x)
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
    int currCursorY = currSelEntry * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int16_t x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, currCursorY, COLOR_GREEN);
        lcd.drawPixel(x, currCursorY + MENU_SELECTION_HEIGHT, COLOR_GREEN);
    }
}

void ScreenMenu::buttonPress()
{
    Screen* menuCurr = nodeCurr->getData();
    ScreenMenu* screenMenuCurr = static_cast<ScreenMenu*>(menuCurr);

    uint16_t iMenuSel = screenMenuCurr->getMenuSelection();

    // go to parent node if "Back" button i.e. the last menu item is selected
    bool isBackBtnSel = (iMenuSel == screenMenuCurr->getNumSubMenus() - 1 && nodeCurr->getParent() != nullptr);
    if (isBackBtnSel)
    {
        Serial.println("[PICO] :: INFO :: menu entered (back)");
        nodeCurr = nodeCurr->getParent();
        menuCurr = nodeCurr->getData();
    }
    else
    {
        std::string menuName = screenMenuCurr->getSubmenuName(iMenuSel);
        if (nodeCurr->getNumChildren() == 0 || nodeCurr->getNumChildren() < iMenuSel + 1)
        {
            // selected menu is not implemented --> draw error msg
            Serial.println("[PICO] :: ERROR :: menu not implemented!");
            Gui::drawErrorNotImpl(menuName);
            return;
        }
        // the menu is implemented --> draw menu
        Serial.print("[PICO] :: INFO :: menu selected: ");
        Serial.println(menuName.c_str());
        nodeCurr = nodeCurr->getChild(iMenuSel);
        menuCurr = nodeCurr->getData();
    }
    Gui::clearScreen();
    menuCurr->draw();
}

uint16_t ScreenMenu::getMenuSelection() { return currSelEntry; }

uint16_t ScreenMenu::getNumSubMenus() { return entries.size(); }

std::string ScreenMenu::getSubmenuName(uint16_t index)
{
    return entries.at(index);
}

