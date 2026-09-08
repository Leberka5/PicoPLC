#include "include/Gui.h"
#include "include/tree/tree.h"


/********************************************************************
 * init static member of Gui class
 ********************************************************************/
Adafruit_ST7789 Gui::lcd = Adafruit_ST7789(SPI_CS, LCD_DC, LCD_RST);
tree<ScreenMenu> Gui::menuTree = tree<ScreenMenu>();
nptr<ScreenMenu> Gui::nodeCurr = nullptr;
ScreenMenu* Gui::menuCurr = nullptr;
ScreenError Gui::screenError = ScreenError(lcd);
bool Gui::inErrorState = false;
bool Gui::menuEntered = false;



/********************************************************************
 * methods of class Gui
 ********************************************************************/
void Gui::enterMenu()
{
    menuEntered = true;
}

void Gui::changeSelection(MenuActions actionGui) { menuCurr->drawSelUpdate(actionGui); }

void Gui::gotoMainMenu()
{
    nodeCurr = menuTree.getRoot();
    menuCurr = nodeCurr->getDataPtr();
    clearScreen();
    menuCurr->draw();
}

void Gui::clearScreen() { lcd.fillScreen(COLOR_BLACK); }

bool Gui::hasMenuEntered() { return menuEntered; }

void Gui::changeMenu()
{
    Serial.println(F("Menu Entered"));
    menuEntered = false;
    int16_t iMenuSel = menuCurr->getMenuSelection();

    // go to parent node if "Back" button i.e. the last menu item is selected
    bool isBackBtnSel = (iMenuSel == menuCurr->getNumSubMenus() - 1 && nodeCurr->getParent() != nullptr);
    if (isBackBtnSel)
    {
        nodeCurr = nodeCurr->getParent();
        menuCurr = nodeCurr->getDataPtr();
    }
    else
    {
        // check if selected menu is even implemented yet
        if (nodeCurr->getNumChildren() == 0 || nodeCurr->getNumChildren() < iMenuSel + 1)
        {
            std::string menuName = menuCurr->getSubmenuName(iMenuSel);
            drawErrorNotImpl(menuName);
        }
        else // the menu is implemented --> draw it!
        {
            nodeCurr = nodeCurr->getChild(iMenuSel);
            menuCurr = nodeCurr->getDataPtr();
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
    delay(50);
    lcd.fillScreen(COLOR_BLACK);
    lcd.setTextColor(ST77XX_WHITE);
    lcd.setTextSize(FONT_12x16);
    // draw main menu
    menuCurr->draw();
}

void Gui::initMenus()
{
    nodeCurr = menuTree.setRoot(ScreenMenu(lcd, "Main Menu", std::vector<std::string>({
        "(1) Analog Read",
        "(2) Protocols"}), false));

    nptr<ScreenMenu> nodeAnalog = nodeCurr->addNode(ScreenMenu(lcd, "Analog Read", std::vector<std::string>({
        "Read Input",
        "Threshold Action"})));

    nodeAnalog->addNode(ScreenMenu(lcd, "Analog Read", std::vector<std::string>({
        "Idk",
        "Idkkk"})));



    menuCurr = nodeCurr->getDataPtr();
}

void Gui::drawErrorNotImpl(std::string menuName)
{
    screenError.setErrorMsg("Menu '"+ menuName +"' not yet implemented!");
    clearScreen();
    screenError.draw();
    delay(5000);
    gotoMainMenu();
}
