#include "Arduino.h"

class MiniTimer
{

private:
    uint16_t interval;
    uint16_t current;
    uint16_t previous;

public:
    void setup(uint16_t interval);
    bool update();
};

class MiniCountdown
{
private:
    bool isRunning = false;     //Default set to false
    uint16_t startTime;
    uint16_t waitingTime;

public:
    void start(uint16_t waitingTime);
    void stop();
    bool update();
    bool getStatus();
    void setStatus(bool value);
};