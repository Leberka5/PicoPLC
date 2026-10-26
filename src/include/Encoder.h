/*
 * class Encoder:
 *  - decodes and debounces the rotary encoder's signals
 *  - readInput() is the main method which returns a EncoderAction
 */

#ifndef DEVBOARD_ENCODER_H
#define DEVBOARD_ENCODER_H

#include "defines.h"

constexpr int32_t DEBUG_ENCODER = 1;
constexpr int32_t DEBOUNCE_DELAY = 70;

enum EncoderAction
{
    NONE = -1,
    LEFT = 0,
    RIGHT = 1,
    BUTTON = 2
};

class Encoder
{
public:
    static void init();
    static EncoderAction readInput();
    static uint64_t timeLastBtnPress;
private:
    static void printCounter();
    static int32_t counter;
    static int32_t lastStateA;
    static int32_t currStateA;
    static uint64_t timeLastAction;
    static uint8_t lastStateEncoded;
    static int8_t subSteps;
    static uint32_t timeLastStateChange;
};

#endif //DEVBOARD_ENCODER_H
