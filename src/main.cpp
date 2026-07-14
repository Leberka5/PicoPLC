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

// setup GUI, GPIOs, INTs, ...
void setup()
{
    // init lcd and encoder and draw menu
    Serial.begin(9600);
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
    EncoderAction actionEnc = Encoder::readInput();
    if (actionEnc == RIGHT)
        Gui::changeSelection(UP);
    else if (actionEnc == LEFT)
        Gui::changeSelection(DOWN);

    if (Gui::hasMenuEntered())
        Gui::changeMenu();
}