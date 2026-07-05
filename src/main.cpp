#include "include/defines.h"
#include "include/Gui.h"
#include "include/Encoder.h"

// global vars
Encoder encoder;

// ISR: rotary encoder button press
INTERRUPT isrEncoderButton()
{
    bool isDebounced = (millis() - encoder.timeLastBtnPress) > DEBOUNCE_DELAY;
    if (isDebounced)
    {
        Gui::enterMenu();
        encoder.timeLastBtnPress = millis();
    }
}

// setup GUI, GPIOs, INTs, ...
void setup()
{
    // init lcd and draw menu
    Gui::init();
    // config encoder
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_BUTTON, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrEncoderButton, RISING);
}

// main loop
void loop()
{
    EncoderAction actionEnc = encoder.readInput();
    if (actionEnc == RIGHT)
        Gui::changeSelection(UP);
    else if (actionEnc == LEFT)
        Gui::changeSelection(DOWN);
}