#include "include/Gui.h"
#include <utility>


// pseudo init of Gui's static member (real init is in Gui::init())
Adafruit_ST7789 Gui::lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
ScreenMenu* Gui::menuCurrent = &screenMain;
ScreenMenu Gui::screenMain = ScreenMenu(lcd, nullptr, "", { });
ScreenError Gui::screenError = ScreenError(lcd, nullptr, "");

ScreenError& ScreenError::operator=(const ScreenError& other)
{
    if (this == &other)
        return *this;
    // copy ALL member of GuiMenu
    this->lcd = other.lcd;
    this->menuTitle = other.menuTitle;
    this->menuParent = other.menuParent;
    this->colorBg = other.colorBg;

    return *this;
}

Screen::Screen(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle) :
    lcd(lcd),
    menuParent(menuParent),
    menuTitle(menuTitle)
{
    colorBg = COLOR_BLACK;
}


ScreenError::ScreenError(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle) :
    Screen(lcd, menuParent, std::move(menuTitle))
{

}


void ScreenError::drawError(std::string errorMsg)
{
    errorLog.emplace_back(errorMsg);
    errorMsgCurr = errorMsg;
    draw();
}

void ScreenError::draw()
{
    lcd.fillScreen(COLOR_BLACK);
    lcd.setTextColor(COLOR_RED);
    lcd.setCursor(0, 50);
    lcd.write(errorMsgCurr.c_str());
}

/**********************************
 * methods of class GuiMenu
 **********************************/
ScreenMenu::ScreenMenu(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle, std::vector<std::string> menuEntries) :
    Screen(lcd, menuParent, std::move(menuTitle)),
    menuEntries(std::move(menuEntries)),
    currSelection(0)
{
    // add "Back" button for all menus, except main menu
    if (menuParent != nullptr)
        this->menuEntries.emplace_back("Back");
}

ScreenMenu& ScreenMenu::operator=(const ScreenMenu& other)
{
    if (this == &other)
        return *this;
    // copy ALL member of GuiMenu
    this->lcd = other.lcd;
    this->menuTitle = other.menuTitle;
    this->menuEntries = other.menuEntries;
    this->subMenus = other.subMenus;
    this->currSelection = other.currSelection;
    this->colorBg = other.colorBg;

    return *this;
}

void ScreenMenu::draw()
{
    uint8_t currCursorY = PADDING_TITLE_Y;
    // draw menu title
    lcd.setTextColor(COLOR_GREEN_2);
    lcd.setTextSize(FONT_12x16); // title'd be better lookin' if bigger, but results in less space duh
    lcd.setCursor(PADDING_TITLE_X, currCursorY);
    lcd.write(menuTitle.c_str());
    // draw menu entries
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(FONT_12x16);
    currCursorY = TITLE_HEIGHT + PADDING_MENU_Y;
    lcd.setCursor(PADDING_MENU_X, currCursorY);
    for (std::string currMenuEntry : menuEntries)
    {
        // write menu entry
        lcd.write(currMenuEntry.c_str());
        // update cursor for next menu entry
        currCursorY += MENU_SELECTION_HEIGHT;
        lcd.setCursor(PADDING_MENU_X, currCursorY + PADDING_MENU_Y);
    }
}

void ScreenMenu::drawSelection(MenuActions actionMenuSel)
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
        currSelection = menuEntries.size() - 1;
    else if (currSelection >= menuEntries.size())
        currSelection = 0;

    // draw new selection
    uint8_t currCursorY = currSelection * MENU_SELECTION_HEIGHT + TITLE_HEIGHT;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        lcd.drawPixel(x, currCursorY, COLOR_GREEN);
        lcd.drawPixel(x, currCursorY + MENU_SELECTION_HEIGHT, COLOR_GREEN);
    }
}

void ScreenMenu::addSubMenu(ScreenMenu newSubMenu) { subMenus.emplace_back(newSubMenu); }

ScreenMenu* ScreenMenu::getSubMenu(uint32_t index) { return &subMenus.at(index); }

ScreenMenu* ScreenMenu::getParentMenu() { return menuParent; }

uint32_t ScreenMenu::getCurrSelection() { return currSelection; }

uint32_t ScreenMenu::getNumSubMenus() { return menuEntries.size(); }


/**********************************
 * methods of class Gui
 **********************************/
void Gui::init()
{
    // (0) main menu
    screenMain = ScreenMenu(lcd, nullptr, "Main Menu", {
        "Timed Actions",
        "INT Actions",
        "ADC",
        "PWM",
        "I2C" });
    // (1): setup menu "Timed Actions"
    ScreenMenu menuTimedActions = ScreenMenu(lcd, &screenMain, "Timed Actions", {
       "Add Timer",
       "Delete Timer" });
    screenMain.addSubMenu(menuTimedActions);
    // TODO: (2): setup menu "INT Actions"

    // create screen to display (and log) error messages
    screenError = ScreenError(lcd, nullptr, "Error");

    // setup Gui class
    menuCurrent = &screenMain;
    // init lcd
    lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
    lcd.init(SCREEN_WIDTH, SCREEN_HEIGHT, SPI_MODE2);
    lcd.setRotation(ROTATION_UP);
    delay(50);
    lcd.fillScreen(COLOR_BLACK);
    lcd.setTextColor(ST77XX_WHITE);
    lcd.setTextSize(FONT_12x16);
    // draw parent menu
    menuCurrent->draw();
    menuCurrent->drawSelection(INIT);
}

// callback func for encoder button press (called in ISR --> keep it simple)
void Gui::enterMenu()
{
    // change current menu
    if (menuCurrent->getCurrSelection() == menuCurrent->getNumSubMenus() - 1)
        menuCurrent = menuCurrent->getParentMenu();
    else
        menuCurrent = menuCurrent->getSubMenu(menuCurrent->getCurrSelection());
    if (menuCurrent == nullptr)
        Gui::screenError.drawError("Screen not implemented yet!");
    // clear screen
    lcd.fillScreen(COLOR_BLACK);
    // draw new menu
    menuCurrent->draw();
    menuCurrent->drawSelection(INIT);
}

void Gui::changeSelection(MenuActions actionGui)
{
    menuCurrent->drawSelection(actionGui);
}

