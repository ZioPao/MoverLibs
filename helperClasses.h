#include "Arduino.h"

enum Direction
{
    LEFT,
    RIGHT,
    FORWARD,
    GENERIC_LATERAL
};

class Motion
{

private:
    int16_t value;
    Direction dir;

public:
    Motion(int16_t value, Direction dir);

    void setValue(int16_t value);
    void modifyValue(int16_t mod);
    int16_t getValue();

    int16_t getAbsValue();


    void setDirection(Direction dir);
    Direction getDirection();
};
