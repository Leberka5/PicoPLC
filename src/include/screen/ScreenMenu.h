#ifndef PICOPLC_SCREENMENU_H
#define PICOPLC_SCREENMENU_H

#include "../defines.h"
#include "../screen/Screen.h"

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


#endif //PICOPLC_SCREENMENU_H
