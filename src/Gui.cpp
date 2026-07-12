#include "include/Gui.h"
#include "include/tree/tree.h"


/**********************************
 * init static member of Gui class
 **********************************/
Adafruit_ST7789 Gui::lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
tree<ScreenMenu> Gui::menuTree = tree<ScreenMenu>();
nptr<ScreenMenu> Gui::nodeCurr = nullptr;
ScreenMenu* Gui::menuCurr = nullptr;
ScreenError Gui::screenError = ScreenError(lcd);
bool Gui::inErrorState = false;


/**********************************
 * Screen base class methods
 **********************************/
Screen::Screen(Adafruit_ST7789& lcd) :
    lcd(lcd),
    colorBg(COLOR_BLACK) { }


/**********************************
 * ScreenError methods
 **********************************/
ScreenError::ScreenError(Adafruit_ST7789& lcd) :
    Screen(lcd) { }

void ScreenError::drawError(std::string errorMsg)
{
    errorLog.emplace_back(errorMsg);
    this->errorMsg = errorMsg;
    draw();
}

void ScreenError::draw()
{
    lcd.setTextColor(COLOR_RED);
    lcd.setCursor(0, 50);
    lcd.write(errorMsg.c_str());
}


/**********************************
 * methods of class GuiMenu
 **********************************/
ScreenMenu::ScreenMenu(Adafruit_ST7789& lcd, std::string menuTitle, std::vector<std::string> menuEntries) :
    Screen(lcd),
    menuTitle(menuTitle),
    menuEntries(menuEntries),
    currSelection(0) { }

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
    for (std::string currMenuEntry : menuEntries)
    {
        // write menu entry
        lcd.write(currMenuEntry.c_str());
        // update cursor for next menu entry
        currCursorY += MENU_SELECTION_HEIGHT;
        lcd.setCursor(PADDING_MENU_X, currCursorY + PADDING_MENU_Y);
    }
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

uint16_t ScreenMenu::getCurrSelection() { return currSelection; }

uint16_t ScreenMenu::getNumSubMenus() { return menuEntries.size(); }


/**********************************
 * methods of class Gui
 **********************************/
void Gui::enterMenu()
{
    // TODO: optimize: ISR must stay small (without inErrorState logic µC crashes ...)

    // ScreenMenu*                    node*
    if (menuCurr->getNumSubMenus() == nodeCurr->getNumNodes())
    {
        int32_t menuIndex = menuCurr->getCurrSelection();
        nodeCurr = nodeCurr->getNode(menuIndex);
        menuCurr = nodeCurr->getDataPtr();
        clearScreen();
        menuCurr->draw();
    }
    else
    {
        inErrorState = true;
    }
}

void Gui::changeSelection(MenuActions actionGui) { menuCurr->drawSelUpdate(actionGui); }

void Gui::clearScreen() { lcd.fillScreen(COLOR_BLACK); }

void Gui::init()
{
    // setup screens
    initMenus();
    // init lcd
    lcd.init(SCREEN_WIDTH, SCREEN_HEIGHT, SPI_MODE2);
    lcd.setRotation(ROTATION_UP);
    delay(50);
    lcd.fillScreen(COLOR_BLACK);
    lcd.setTextColor(ST77XX_WHITE);
    lcd.setTextSize(FONT_12x16);
    // draw main menu
    menuCurr->draw();
    menuCurr->drawSelUpdate(INIT);
}

void Gui::initMenus()
{
    nodeCurr = menuTree.addNode(ScreenMenu(
        lcd, "Main Menu", std::vector<std::string>({
        "(1) Analog Read",
        "(2) Protocols"} )));
    nodeCurr->addNode(ScreenMenu(
        lcd, "Analog Read", std::vector<std::string>({
        "test"})));
    nodeCurr->addNode(ScreenMenu(
        lcd, "Protocols", std::vector<std::string>({
        "test"})));
    // TODO: recursively add "Back" button/node to submenus/subnodes
    menuCurr = nodeCurr->getDataPtr();
}
