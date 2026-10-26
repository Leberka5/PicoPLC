#ifndef PICOPLC_SCREENPWM_H
#define PICOPLC_SCREENPWM_H

#include "../defines.h"
#include "../screen/Screen.h"

class ScreenPwm : public Screen
{
public:
    ScreenPwm(Adafruit_ST7789& lcd, node_ptr<Screen*>& nodeCurr, const std::string &menuTitle, std::vector<std::string> entries);
    ScreenPwm& operator=(const ScreenPwm& other);
    void draw() override;
    void drawSelUpdate(MenuActions actionMenuSel) override;
    void buttonPress() override;
private:
};

#endif //PICOPLC_SCREENPWM_H
