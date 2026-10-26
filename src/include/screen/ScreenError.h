#ifndef PICOPLC_SCREENERROR_H
#define PICOPLC_SCREENERROR_H

#include "../defines.h"
#include "../screen/Screen.h"

/********************************************************************
 * display errors:
 *  - trying to enter not defined sub menu
 ********************************************************************/
class ScreenError : public Screen
{
public:
    ScreenError(Adafruit_ST7789& lcd, node_ptr<Screen*>& nodeCurr);
    void setErrorMsg(const std::string& errorMsg);
    void draw() override;
    void drawSelUpdate(MenuActions actionMenuSel) override;
    void buttonPress() override;
private:
    std::string errorMsg;
    std::vector<std::string> errorLog;
};

#endif //PICOPLC_SCREENERROR_H
