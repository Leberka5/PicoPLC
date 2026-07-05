#ifndef GUI_H
#define GUI_H

#include "include/defines.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// just some mappings for options of Adafruit_ST7789
enum LcdNumerics
{
    FONT_6x8 = 1,
    FONT_12x16 = 2,
    FONT_18x24 = 3,
    ROTATION_UP = 2
};

// 16 bit colors for Adafruit_ST7789
enum GuiColors
{
    COLOR_WHITE = 0xFFFF,
    COLOR_BLACK = 0x0000,
    COLOR_GREEN = 0x0f00,
    COLOR_GREEN_2 = 0x07E0,
    COLOR_RED = 0xF800,
    COLOR_BLUE = 0x001F,
    COLOR_BLUEGREY = 0x22ED,
    COLOR_CYAN = 0x07FF,
    COLOR_MAGENTA = 0xF81F,
    COLOR_YELLOW = 0xFFE0,
    COLOR_ORANGE = 0xFC00
};

// metrics for a 240x240 lcd display
enum GuiNumerics
{
    SCREEN_WIDTH = 240,
    SCREEN_HEIGHT = 240,
    FONT_HEIGHT_1 = 8,      // name refers to val LcdNumerics::FONT_XXX
    FONT_HEIGHT_2 = 16,     // ...
    FONT_HEIGHT_3 = 24,     // ...
    PADDING_TITLE_X = 30,   // delta x for title
    PADDING_TITLE_Y = 6,    // ... y ...
    PADDING_MENU_X = 4,
    PADDING_MENU_Y = 4,
    TITLE_HEIGHT = FONT_HEIGHT_3 + 2*PADDING_TITLE_Y,
    MENU_SELECTION_HEIGHT = FONT_HEIGHT_2 + 2*PADDING_MENU_Y    // height for encoder sel in GUI
};

// used to change the currently selected menu entry (via encoder)
enum MenuActions
{
    INIT,
    DOWN,
    UP,
};

class ScreenMenu;
class ScreenError;


class Screen
{
public:
    Screen(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle);
    virtual ~Screen() = default; // virtual in order to call derived class's destructor when deleted
    virtual void draw() = 0;
protected:
    Adafruit_ST7789& lcd;
    ScreenMenu* menuParent;
    std::string menuTitle;
    uint16_t colorBg;
};

class ScreenError : public Screen
{
public:
    ScreenError(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle);
    ScreenError& operator=(const ScreenError&);
    void drawError(std::string errorMsg);
    void draw() override;
private:
    std::string errorMsgCurr;
    std::vector<std::string> errorLog;
};

class ScreenMenu : public Screen
{
public:
    ScreenMenu(Adafruit_ST7789& lcd, ScreenMenu* menuParent, std::string menuTitle, std::vector<std::string> menuEntries);
    ScreenMenu& operator=(const ScreenMenu& other);
    void draw() override;                           // draw them friggin menu entries
    void drawSelection(MenuActions actionMenuSel);  // draw curr encoder sel
    void addSubMenu(ScreenMenu newSubMenu);
    ScreenMenu* getSubMenu(uint32_t index);
    ScreenMenu* getParentMenu();
    uint32_t getCurrSelection();        // ret curr enc sel
    uint32_t getNumSubMenus();
private:
    // TODO: adjust copy constructor for every new member
    std::vector<ScreenMenu> subMenus;          // sub menus of curr menu
    std::vector<std::string> menuEntries;   // sub menu names
    int64_t currSelection;                  // curr encoder selection
};


/*
 * supervisor class (therefore static)
 * containing all its menus and lcd instance
 */
class Gui
{
public:
    static void init();
    static void enterMenu();                           // callback func for btn press
    static void changeSelection(MenuActions actionGui);  // wrapper for GuiMenu::drawSelection
private:
    static Adafruit_ST7789 lcd;
    static ScreenMenu* menuCurrent;    // currently displayed menu
    static ScreenMenu screenMain;        // the most  parent menu of all
    static ScreenError screenError;        // the most  parent menu of all
};

#endif //GUI_H
