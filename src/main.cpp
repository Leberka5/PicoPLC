#include "include/defines.h"
#include "include/Gui.h"
#include "include/Encoder.h"
#include "pico/bootrom.h"

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

static void reset_system()
{
    reset_usb_boot(0, 0);
}

// setup GUI, GPIOs, INTs, ...
void setup()
{
    // init lcd and encoder and draw menu
    Serial.begin(115200);
    Encoder::init();
    Gui::init();
    // config encoder
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_BUTTON, INPUT_PULLDOWN);
    pinMode(pins::LED, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrEncoderButton, RISING);
}

// main loop
void loop()
{
    // check if menu switch
    EncoderAction actionEnc = Encoder::readInput();
    if (actionEnc == RIGHT)
        Gui::changeSelection(UP);
    else if (actionEnc == LEFT)
        Gui::changeSelection(DOWN);
    if (Gui::hasMenuEntered())
        Gui::changeMenu();

    // check for received commands (PC)
    if (Serial.available() > 0)
    {
        String strRecvPC = Serial.readStringUntil('\n');
        strRecvPC.trim();
        if (strRecvPC == "reboot")
        {
            Serial.println("[PICO] :: received command: reboot");
            reset_system();
        }
    }
}
