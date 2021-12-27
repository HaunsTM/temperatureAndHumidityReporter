#include "OnboardLED.h"

#ifndef STATE_AND_DURATION_H
#define STATE_AND_DURATION_H

class StateAndDuration {

public:
    StateAndDuration(OnboardLED::LEDState ledState, unsigned int stateLengthMs);
    StateAndDuration() = default; // similar to create a default constructor

    OnboardLED::LEDState ledState();

    unsigned int stateLengthMs();

private:
    OnboardLED::LEDState _ledState;
    unsigned int _stateLengthMs;
};

#endif