#include <Arduino.h>
#include "OnboardLED.h"
#include "StateAndDuration.h"
#include "Vector.h"

#ifndef BLINKER_H
#define BLINKER_H

class Blinker {

public:
    Blinker(OnboardLED& onboardLED);

    void initialize();

    enum class BlinkerSequence {
        EMPTY,
        ERROR,
        HTTP_REQUEST,
        PERFORMING_MOVE,
        PAUSE_1_S,
        STAND_BY,
        STAND_BY_NON_WIFI
    };

    void start();
    void stop();

    void setBlinkerSequence(BlinkerSequence blinkerSequence);
    void handleBlinker();


private:

    static const int ELEMENT_COUNT_MAX = 10;
    typedef Vector<StateAndDuration> Elements;
    StateAndDuration _blinkSequenceArray[ELEMENT_COUNT_MAX];
    Elements _currentlyPerformingBlinkerSequenceList;

    bool _initialized;
    bool _isRunning;
    OnboardLED _onboardLED;

    BlinkerSequence _previousBlinkerSequence;
    static const int INITIAL_STEP_TO_PERFORM = 0;
    unsigned long _currentlyPerformingBlinkerSequenceStepStartedAtMillis;
    int _currentlyPerformingBlinkerSequenceStepIndex;

    void set(OnboardLED::LEDState state);

    void setSequenceErrorState();
    void setSequenceHttpRequest();
    void setSequencePause1s();
    void setSequencePerformingMove();
    void setSequenceStandBy();
    void setSequenceStandByNoWifi();


    void performBlinkSequence();
    int nextCalculatedStepIndex(int currentStepIndex);
};

#endif