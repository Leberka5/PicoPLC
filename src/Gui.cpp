#include "include/Gui.h"
#include "include/tree/tree.h"


/********************************************************************
 * init static member of Gui class
 ********************************************************************/
Adafruit_ST7789 Gui::lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
tree<Screen*> Gui::menuTree = tree<Screen*>();
node_ptr<Screen*> Gui::nodeCurr = nullptr;
Screen* Gui::menuCurr = nullptr;
ScreenError Gui::screenError = ScreenError(lcd, Gui::nodeCurr);
bool Gui::inErrorState = false;
bool Gui::isButtonPressed = false;


/********************************************************************
 * methods of class Gui
 ********************************************************************/
void Gui::setButtonPressed() { isButtonPressed = true; }

void Gui::changeSelection(const MenuActions actionGui) { menuCurr->drawSelUpdate(actionGui); }

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
    isButtonPressed = false;
    nodeCurr->getData()->buttonPress();
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

    nodeCurr = menuTree.setRoot(new ScreenMenu(lcd, nodeCurr, "Main Menu", std::vector<std::string>({
        "(1) PWM Generator",
        "(2) Analog Read"}), false));

    node_ptr<Screen*> nodePwm = nodeCurr->addNode(new ScreenMenu(lcd, nodeCurr,"PWM Generator", std::vector<std::string>({
        "Set Frequency",
        "On/Off"})));

    nodePwm->addNode(new ScreenPwm(lcd, nodeCurr, "Set Frequency", std::vector<std::string>({ "100" })));

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
