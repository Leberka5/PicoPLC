#include "include/Encoder.h"


/**********************************
 * init static member of Encoder class
 **********************************/
int32_t Encoder::lastStateA;
int32_t Encoder::currStateA;
int32_t Encoder::counter = 0;
uint64_t Encoder::timeLastAction = 0;
uint64_t Encoder::timeLastBtnPress = 0;
uint8_t Encoder::lastStateEncoded = 0;
int8_t Encoder::subSteps = 0;
uint32_t Encoder::timeLastStateChange = 0;


/**********************************
 * Encoder methods
 **********************************/
void Encoder::init()
{
    lastStateA = digitalRead(ENCODER_A);
}

void Encoder::printCounter()
{
    Serial.print("[PICO] :: ENCODER :: ");
    Serial.println(counter);
}

EncoderAction Encoder::readInput()
{
    EncoderAction action = NONE;

    // read inputs and code states
    bool stateA = digitalRead(ENCODER_A);
    bool stateB = digitalRead(ENCODER_B);
    uint8_t encoded = (stateA << 1) | stateB;
    // cancel if state has not changed
    if (encoded == lastStateEncoded)
        return action;
    // noise filter for contact bouncing in encoder
    if ((millis() - timeLastStateChange) < 2)
        return action;
    timeLastStateChange = millis();
    // calc state transition
    uint8_t sum = (lastStateEncoded << 2) | encoded;
    // right rotation detected
    if (sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000)
        subSteps++;
    // left rotation detected
    else if (sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100)
        subSteps--;
    // save state for next method call
    lastStateEncoded = encoded;

    // apply action
    // stepsPerClick = (2 | 4) depending on encoder model
    constexpr int8_t stepsPerClick = 4;
    if (subSteps >= stepsPerClick)
    {
        counter--;
        action = LEFT;
        subSteps = 0;
        printCounter();
    }
    else if (subSteps <= -stepsPerClick)
    {
        counter++;
        action = RIGHT;
        subSteps = 0;
        printCounter();
    }

    return action;
}
