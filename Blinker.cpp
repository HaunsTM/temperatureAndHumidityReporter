#include <Arduino.h>
#include "Blinker.h"
#include "OnboardLED.h"
#include "StateAndDuration.h"
#include "Vector.h"

Blinker::Blinker(OnboardLED& onboardLED) {
    _onboardLED = onboardLED;
    _initialized = false;
}

void Blinker::initialize() {

    if (!_initialized) {

        _currentlyPerformingBlinkerSequenceList.setStorage(_blinkSequenceArray);

        _isRunning = false;
        _onboardLED.setOff();

        _previousBlinkerSequence = BlinkerSequence::EMPTY;
        _currentlyPerformingBlinkerSequenceStepIndex = INITIAL_STEP_TO_PERFORM;
        _initialized = true;
    }
}

void Blinker::setSequenceErrorState() {

    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOffShort(OnboardLED::LEDState::OFF, 50);
    StateAndDuration ledOff(OnboardLED::LEDState::OFF, 750);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOff);
};

void Blinker::setSequenceHttpRequest() {
    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOffShort(OnboardLED::LEDState::OFF, 50);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
};

void Blinker::setSequencePause1s() {

    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOffShort(OnboardLED::LEDState::OFF, 50);
    StateAndDuration ledOff(OnboardLED::LEDState::OFF, 850);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOff);
};

void Blinker::setSequencePerformingMove() {

    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOffLong(OnboardLED::LEDState::OFF, 450);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffLong);
};

void Blinker::setSequenceStandBy() {

    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOff(OnboardLED::LEDState::OFF, 4950);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOff);
};

void Blinker::setSequenceStandByNoWifi() {

    StateAndDuration ledOn(OnboardLED::LEDState::ON, 50);
    StateAndDuration ledOffShort(OnboardLED::LEDState::OFF, 50);
    StateAndDuration ledOffLong(OnboardLED::LEDState::OFF, 4750);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffShort);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOn);
    _currentlyPerformingBlinkerSequenceList.push_back(ledOffLong);
};

void Blinker::set(OnboardLED::LEDState state) {

    switch (state) {
        case OnboardLED::LEDState::ON:
            _onboardLED.setOn();
            break;
        case OnboardLED::LEDState::OFF:
            _onboardLED.setOff();
            break;
    }

};

int Blinker::nextCalculatedStepIndex(int currentStepIndex) {

    int blinkSequenceListSize = _currentlyPerformingBlinkerSequenceList.size();
    int maxStepIndex = blinkSequenceListSize - 1;

    if (currentStepIndex + 1 <= maxStepIndex) {

        return currentStepIndex + 1;
    };
    return INITIAL_STEP_TO_PERFORM;
}

void Blinker::performBlinkSequence() {

    unsigned long currentMillis = millis();
    unsigned int interval = _currentlyPerformingBlinkerSequenceList.at(_currentlyPerformingBlinkerSequenceStepIndex).stateLengthMs();
    unsigned long nextStepCanBePerformedAtMillis = _currentlyPerformingBlinkerSequenceStepStartedAtMillis + interval;
    const bool timeIsDueToPerformNextStep = nextStepCanBePerformedAtMillis < currentMillis;
    
    int blinkSequenceListSize = _currentlyPerformingBlinkerSequenceList.size();

    if (timeIsDueToPerformNextStep) {

        int nextStepIndex = nextCalculatedStepIndex(_currentlyPerformingBlinkerSequenceStepIndex);
        OnboardLED::LEDState ledStateForNextStep = _currentlyPerformingBlinkerSequenceList.at(nextStepIndex).ledState();

        set(ledStateForNextStep);
        _currentlyPerformingBlinkerSequenceStepStartedAtMillis = currentMillis;
        _currentlyPerformingBlinkerSequenceStepIndex = nextStepIndex;
    }
};


void Blinker::start() {

    _currentlyPerformingBlinkerSequenceStepIndex = INITIAL_STEP_TO_PERFORM;
    _isRunning = true;

};

void Blinker::stop() {

    set(OnboardLED::LEDState::OFF);
    _isRunning = false;

};

void Blinker::setBlinkerSequence(BlinkerSequence currentBlinkerSequence) {

    const bool blinkerSequenceHasChanged = _previousBlinkerSequence != currentBlinkerSequence;

    if (blinkerSequenceHasChanged) {

        _previousBlinkerSequence = currentBlinkerSequence;
        _currentlyPerformingBlinkerSequenceStepIndex = INITIAL_STEP_TO_PERFORM;
        _currentlyPerformingBlinkerSequenceList.clear();

        switch (currentBlinkerSequence)
        {
            case Blinker::BlinkerSequence::EMPTY:
                break;

            case Blinker::BlinkerSequence::ERROR:
                setSequenceErrorState();
                break;

            case Blinker::BlinkerSequence::HTTP_REQUEST:
                setSequenceHttpRequest();
                break;

            case Blinker::BlinkerSequence::PAUSE_1_S:
                setSequencePause1s();
                break;

            case Blinker::BlinkerSequence::PERFORMING_MOVE:
                setSequencePerformingMove();
                break;

            case Blinker::BlinkerSequence::STAND_BY:
                setSequenceStandBy();
                break;

            case Blinker::BlinkerSequence::STAND_BY_NON_WIFI:
                setSequenceStandByNoWifi();
                break;

            default:
                setSequenceErrorState();
                break;
        }

    }
};

void Blinker::handleBlinker() {

    if (_isRunning) {
        performBlinkSequence();
    }

};