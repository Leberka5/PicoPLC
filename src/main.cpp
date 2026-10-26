#include <pico/bootrom.h>
#include <pico/multicore.h>

#include "include/defines.h"
#include "include/Gui.h"
#include "include/Encoder.h"

// ISR: rotary encoder button press
INTERRUPT isrEncoderButton()
{
    // TODO: keep as small as possible
    bool isDebounced = (millis() - Encoder::timeLastBtnPress) > DEBOUNCE_DELAY;
    if (isDebounced)
    {
        Gui::enterMenu();
        Encoder::timeLastBtnPress = millis();
    }
}

// check if current menu selection switched (encoder rotation)
inline void check_menu_switch()
{
    EncoderAction actionEnc = Encoder::readInput();
    if (actionEnc == RIGHT)
        Gui::changeSelection(UP);
    else if (actionEnc == LEFT)
        Gui::changeSelection(DOWN);
}

// check if menu has been entered (encoder btn press)
inline void check_menu_enter()
{
    if (Gui::hasMenuEntered())
        Gui::changeMenu();
}

// check for received commands (PC)
inline void recv_cmd()
{
    if (Serial.available() > 0)
    {
        String strRecvPC = Serial.readStringUntil('\n');
        strRecvPC.trim();
        if (strRecvPC == "reboot")
        {
            Serial.println("[PICO] :: received command: reboot");
            reset_usb_boot(0, 0);
        }
    }
}

/********************************************************************
 * setup function
 ********************************************************************/
void setup()
{
    // setup serial, encoder and GUI
    Serial.begin(115200);
    Encoder::init();
    Gui::init();
    // setup pins and interrupt for encoder
    pinMode(LED, OUTPUT);
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_BUTTON, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrEncoderButton, RISING);
}

/********************************************************************
 * main loop
 ********************************************************************/
void loop()
{
    check_menu_switch();
    check_menu_enter();
    recv_cmd();
}
