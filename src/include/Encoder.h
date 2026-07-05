/*
 * class Encoder:
 *  - decodes and debounces the rotary encoder's signals
 *  - readInput() is the main method which returns a EncoderAction
 */

#ifndef DEVBOARD_ENCODER_H
#define DEVBOARD_ENCODER_H

#include "defines.h"

constexpr int32_t DEBUG_ENCODER = 1;
// constexpr int32_t DEBOUNCE_DELAY = 50;
constexpr int32_t DEBOUNCE_DELAY = 100;

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
    Encoder();
    EncoderAction readInput();
    // button vars
    uint64_t timeLastBtnPress;
private:
    // encoder vars
    int32_t counter = 0;
    int32_t lastStateA;
    int32_t currStateA;
    uint64_t timeLastAction = 0;
};

#endif //DEVBOARD_ENCODER_H
