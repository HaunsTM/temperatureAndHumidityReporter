#include <Arduino.h>
#include "OnboardLED.h"

OnboardLED::OnboardLED() {
    _initialized = false;
}

void OnboardLED::initialize() {
    if (!_initialized) {
        pinMode(LED_BUILTIN, OUTPUT);
        setOff();

        _initialized = true;
    }
}

int OnboardLED::getCurrentState() {
    return digitalRead(LED_BUILTIN);
}

bool OnboardLED::setOn() {
    digitalWrite(LED_BUILTIN, LOW); // Turn LED on
    return true;
}

bool OnboardLED::setOff() {
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED off
    return true;
}
