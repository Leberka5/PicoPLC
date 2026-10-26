#ifndef GUI_H
#define GUI_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "include/defines.h"
#include "tree/tree.h"
#include "screen/Screen.h"
#include "screen/ScreenError.h"
#include "screen/ScreenMenu.h"
#include "screen/ScreenPwm.h"


enum ErrorCodes
{
    ERROR_NO_SUB_MENUS = -1     // must stay -1 (l. at  ScreenMenu::getCurrSelection)
};


/**********************************
 * supervisor class (therefore static)
 **********************************/
class Gui
{
public:
    static void init();
    static void buttonPress();                            // callback func for btn press (keep as small as possible)
    static void changeSelection(MenuActions actionGui); // wrapper for GuiMenu::drawSelection
    static void gotoMainMenu();
    static void clearScreen();
    static bool hasButtonPressed();
    static void setButtonPressed();
    static void drawErrorNotImpl(const std::string& menuName);
    static Screen* menuCurr;
private:
    static void initMenus();
    static Adafruit_ST7789 lcd;
    static ScreenError screenError;
    static node_ptr<Screen*> nodeCurr;
    static tree<Screen*> menuTree;
    static bool inErrorState;       // true if entering menu which does not exist
    static bool isButtonPressed;
};

#endif //GUI_H
