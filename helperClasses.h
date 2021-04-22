#include "Arduino.h"

enum Direction
{
    LEFT,
    RIGHT,
    FORWARD,
    GENERIC_LATERAL,
    NONE
};

class Motion
{

private:
    int16_t value;
    Direction dir;

public:
    Motion(int16_t value, Direction dir);
    Motion();
    ~Motion();

    void setValue(int16_t value);
    void modifyValue(int16_t mod);
    int16_t getValue();

    int16_t getAbsValue();


    void setDirection(Direction dir);
    Direction getDirection();
};

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
    bool hasFinished = false;
    uint16_t startTime;
    uint16_t waitingTime;
    Direction dir;

public:
    void start(uint16_t waitingTime, Direction dir);
    void stop();
    void reset();
    void update();
    bool getIsRunning();
    bool getHasFinished();
    Direction getDirection();
    void setStatus(bool value);
};