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
    ScreenMenu(Adafruit_ST7789& lcd, node_ptr<Screen*>& nodeCurr, const std::string &menuTitle, const std::vector<std::string> &menuEntries, bool hasBackBtn = true);
    ScreenMenu& operator=(const ScreenMenu& other);
    void draw() override;
    void drawSelUpdate(MenuActions actionMenuSel) override;
    void buttonPress() override;
    uint16_t getMenuSelection();
    uint16_t getNumSubMenus();
    std::string getSubmenuName(uint16_t index);
private:

};


#endif //PICOPLC_SCREENMENU_H
