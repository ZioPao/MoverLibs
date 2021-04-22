#include "helperClasses.h"

void Motion::setValue(int16_t value)
{
    Motion::value = value;
}
void Motion::modifyValue(int16_t mod){
    Motion::value += mod;
}

int16_t Motion::getValue()
{
    return value;
}
int16_t Motion::getAbsValue()
{
    return abs(value);
}

void Motion::setDirection(Direction dir)
{
    Motion::dir = dir;
}

Direction Motion::getDirection()
{
    return dir;
}