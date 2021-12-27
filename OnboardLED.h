#ifndef ONBOARD_LED_H
#define ONBOARD_LED_H

class OnboardLED {

private:
    bool _initialized;

public:
    OnboardLED();

    void initialize();

    int getCurrentState();
    bool setOn();
    bool setOff();

    enum class LEDState {
        ON,
        OFF
    };
};

#endif