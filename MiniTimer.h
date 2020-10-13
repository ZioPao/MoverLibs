

#include "Arduino.h"

class MiniTimer
{

private:
    uint16_t interval;
    uint16_t current;
    uint16_t previous;

    bool status;

public:
    void setup(uint16_t interval);
    bool update();
};