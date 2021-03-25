#pragma once
#include "hprocess.h"
#include "signal.h"
#include "ownpin.h"
class Trigger :
    public Process,
    public OwnPin
{
    int counter = 0;
    int counterMax = 10;
public:
    enum class State:int {
        PUSHED=1,
        RELEASED=0
    };
    Signal<void, State> stateChanged;
    Signal<void> released;
    bool isPushed();
    Trigger(int);
    uint8_t init() override;
    void work() override;
private:
    State currentState = State::RELEASED;
};
#ifdef ARDUINO_IDE 
#include "trigger.cpp" 
#endif
