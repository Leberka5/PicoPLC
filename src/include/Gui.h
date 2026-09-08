#ifndef GUI_H
#define GUI_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "include/defines.h"
#include "tree/tree.h"
#include "screen/screen.h"


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
    static inline void drawErrorNotImpl(std::string menuName);
    static ScreenError screenError;
    static ScreenMenu* menuCurr;
    static Adafruit_ST7789 lcd;
    static tree<ScreenMenu> menuTree;
    static nptr<ScreenMenu> nodeCurr;
    static bool inErrorState;       // true if entering menu which does not exist
    static bool menuEntered;
};

#endif //GUI_H
