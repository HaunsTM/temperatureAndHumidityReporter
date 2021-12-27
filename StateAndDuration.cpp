#include "StateAndDuration.h"

StateAndDuration::StateAndDuration(OnboardLED::LEDState ledState, unsigned int stateLengthMs) {
    _ledState = ledState;
    _stateLengthMs = stateLengthMs;
};

OnboardLED::LEDState StateAndDuration::ledState() {
    return _ledState;
};

unsigned int StateAndDuration::stateLengthMs() {
    return _stateLengthMs;
};