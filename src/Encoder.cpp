#include "include/Encoder.h"

/**********************************
 * init static member of Encoder class
 **********************************/
int32_t Encoder::lastStateA;
int32_t Encoder::currStateA;
int32_t Encoder::counter = 0;
uint64_t Encoder::timeLastAction = 0;
uint64_t Encoder::timeLastBtnPress = 0;



/**********************************
 * Encoder methods
 **********************************/
void Encoder::init()
{
    lastStateA = digitalRead(ENCODER_A);
}

EncoderAction Encoder::readInput()
{
    EncoderAction action = NONE;
    // Detect rotary encoder direction
    currStateA = digitalRead(ENCODER_A);

    // debounce input
    bool applyAction = (millis() - timeLastAction) > DEBOUNCE_DELAY;
    if (!applyAction)
    {
        lastStateA = currStateA;
        return action;
    }

    // check whether left or right rotation has happened
    if (currStateA != lastStateA)
    {
        // right rot
        if (digitalRead(ENCODER_B) != currStateA)
        {
            timeLastAction = millis();
            counter++;
            action = RIGHT;
            #if DEBUG_ENCODER
                Serial.print("-- Direction: Forward, Counter: ");
            #endif
        }
        // left rot
        else
        {
            timeLastAction = millis();
            action = LEFT;
            counter--;
            #if DEBUG_ENCODER
                Serial.print("-- Direction: Backward, Counter: ");
            #endif
        }
        #if DEBUG_ENCODER
            Serial.println(counter);
        #endif
    }
    // save last state to det next rot
    lastStateA = currStateA;

    return action;
}