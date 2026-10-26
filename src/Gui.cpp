#include "include/Gui.h"
#include "include/tree/tree.h"


/********************************************************************
 * init static member of Gui class
 ********************************************************************/
Adafruit_ST7789 Gui::lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
tree<Screen*> Gui::menuTree = tree<Screen*>();
node_ptr<Screen*> Gui::nodeCurr = nullptr;
Screen* Gui::menuCurr = nullptr;
ScreenError Gui::screenError = ScreenError(lcd);
bool Gui::inErrorState = false;
bool Gui::isButtonPressed = false;


/********************************************************************
 * methods of class Gui
 ********************************************************************/
void Gui::setButtonPressed()
{
    isButtonPressed = true;
}

void Gui::changeSelection(const MenuActions actionGui)
{
    menuCurr->drawSelUpdate(actionGui);
}

void Gui::gotoMainMenu()
{
    nodeCurr = menuTree.getRoot();
    menuCurr = nodeCurr->getData();
    clearScreen();
    menuCurr->draw();
}

void Gui::clearScreen() { lcd.fillScreen(COLOR_BLACK); }

bool Gui::hasButtonPressed() { return isButtonPressed; }

void Gui::buttonPress()
{
    ScreenMenu* curr = static_cast<ScreenMenu*>(menuCurr);

    isButtonPressed = false;
    int16_t iMenuSel = curr->getMenuSelection();

    // go to parent node if "Back" button i.e. the last menu item is selected
    bool isBackBtnSel = (iMenuSel == curr->getNumSubMenus() - 1 && nodeCurr->getParent() != nullptr);
    if (isBackBtnSel)
    {
        Serial.println("[PICO] :: INFO :: menu entered (back)");
        nodeCurr = nodeCurr->getParent();
        menuCurr = nodeCurr->getData();
    }
    else
    {
        std::string menuName = curr->getSubmenuName(iMenuSel);
        // check if selected menu is even implemented yet
        if (nodeCurr->getNumChildren() == 0 || nodeCurr->getNumChildren() < iMenuSel + 1)
        {
            Serial.println("[PICO] :: ERROR :: menu not implemented!");
            drawErrorNotImpl(menuName);
        }
        else // the menu is implemented --> draw it!
        {
            Serial.print("[PICO] :: INFO :: menu selected: ");
            Serial.println(menuName.c_str());
            nodeCurr = nodeCurr->getChild(iMenuSel);
            menuCurr = nodeCurr->getData();
        }
    }
    clearScreen();
    menuCurr->draw();
}

void Gui::init()
{
    // setup screens
    initMenus();
    // init lcd
    lcd.init(SCREEN_WIDTH, SCREEN_HEIGHT, SPI_MODE2);
    lcd.setRotation(ROTATION_UP);
    lcd.fillScreen(COLOR_BLACK);
    lcd.setTextColor(ST77XX_WHITE);
    lcd.setTextSize(FONT_12x16);
    // draw main menu
    menuCurr->draw();
}

void Gui::initMenus()
{
    nodeCurr = menuTree.setRoot(new ScreenMenu(lcd, "Main Menu", std::vector<std::string>({
        "(1) PWM Generator",
        "(2) Analog Read"}), false));

    node_ptr<Screen*> nodePwm = nodeCurr->addNode(new ScreenMenu(lcd, "Analog Read", std::vector<std::string>({
        "Set Frequency",
        "On/Off"})));

    nodePwm->addNode(new ScreenPwm(lcd, "Set Frequency", std::vector<std::string>({ "100" })));

    menuCurr = nodeCurr->getData();
}

void Gui::drawErrorNotImpl(const std::string& menuName)
{
    screenError.setErrorMsg("Menu '"+ menuName +"' not yet implemented!");
    clearScreen();
    screenError.draw();
    delay(2000);
    gotoMainMenu();
}
