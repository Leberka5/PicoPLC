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
    static void enterMenu();                            // callback func for btn press (keep as small as possible)
    static void changeSelection(MenuActions actionGui); // wrapper for GuiMenu::drawSelection
    static void gotoMainMenu();
    static void clearScreen();
    static bool hasMenuEntered();
    static void changeMenu();
private:
    static void initMenus();
    static inline void drawErrorNotImpl(const std::string& menuName);
    static Adafruit_ST7789 lcd;
    static ScreenError screenError;
    static Screen* menuCurr;
    static node_ptr<Screen*> nodeCurr;
    static tree<Screen*> menuTree;
    static bool inErrorState;       // true if entering menu which does not exist
    static bool menuEntered;
};

#endif //GUI_H
