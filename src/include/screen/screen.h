#ifndef PICOPLC_SCREEN_H
#define PICOPLC_SCREEN_H

#include "../defines.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>


// just some mappings for options of Adafruit_ST7789
enum LcdNumerics
{
    FONT_6x8 = 1,
    FONT_12x16 = 2, // currently in usee
    FONT_18x24 = 3,
    ROTATION_UP = 2
};

// metrics for a 240x240 lcd display
enum GuiNumerics
{
    SCREEN_WIDTH = 240,
    SCREEN_HEIGHT = 240,
    FONT_HEIGHT_1 = 8,      // name refers to val LcdNumerics::FONT_XXX
    FONT_HEIGHT_2 = 16,
    FONT_HEIGHT_3 = 24,
    FONT_WIDTH_1 = 6,       // name refers to val LcdNumerics::FONT_XXX
    FONT_WIDTH_2 = 12,
    FONT_WIDTH_3 = 18,
    PADDING_TITLE_X = 30,   // delta x for title
    PADDING_TITLE_Y = 6,    // delta y for title
    PADDING_MENU_X = 4,
    PADDING_MENU_Y = 4,
    TITLE_HEIGHT = FONT_HEIGHT_3 + 2*PADDING_TITLE_Y,
    MENU_SELECTION_HEIGHT = FONT_HEIGHT_2 + 2*PADDING_MENU_Y    // height for encoder sel in GUI
};


/********************************************************************
* base class for ScreenMenu, ScreenError
 * override draw() in oder to use it
 ********************************************************************/
class Screen
{
public:
    Screen(Adafruit_ST7789& lcd);
    virtual ~Screen() = default; // virtual in order to call derived class's destructor when deleted
    virtual void draw() = 0;
protected:
    uint16_t centerText(uint16_t numChars);
    Adafruit_ST7789& lcd;
    uint16_t colorBg;
};

/********************************************************************
 * display errors:
 *  - trying to enter not defined sub menu
 ********************************************************************/
class ScreenError : public Screen
{
public:
    ScreenError(Adafruit_ST7789& lcd);
    void setErrorMsg(std::string errorMsg);
    void draw() override;
private:
    std::string errorMsg;
    std::vector<std::string> errorLog;
};


/********************************************************************
 * class used to display menus (row structured)
 ********************************************************************/
class ScreenMenu : public Screen
{
public:
    ScreenMenu(Adafruit_ST7789& lcd, const std::string &menuTitle, const std::vector<std::string> &menuEntries, bool hasBackBtn = true);
    ScreenMenu& operator=(const ScreenMenu& other);
    void draw() override;                           // draw them friggin menu entries
    void drawSelUpdate(MenuActions actionMenuSel);  // draw curr encoder sel
    uint16_t getMenuSelection();
    uint16_t getNumSubMenus();
    std::string getSubmenuName(uint16_t index);
private:
    std::vector<std::string> submenuNames;
    std::string menuTitle;
    int16_t currSelection;
};



#endif //PICOPLC_SCREEN_H
