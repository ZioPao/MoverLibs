#include "helperClasses.h"
/** MOTION**/

Motion::Motion(int16_t value, Direction dir){
    this-> value = value;
    this->dir = dir;
}

Motion::Motion(){}

void Motion::setValue(int16_t value)
{
    this->value = value;
}
void Motion::modifyValue(int16_t mod)
{
    this->value += mod;
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

/** MINI TIMER**/

void MiniTimer::setup(uint16_t interval)
{
    this->interval = interval;
}

//Public methods

/** Updates its status
* @return boolean which determines whether or not continue
*/
bool MiniTimer::update()
{

    current = millis();

    if (current - previous > interval)
    {
        previous = current;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////

/** MINI COUNTDOWN**/

void MiniCountdown::start(uint16_t waitingTime, Direction dir)
{
    this->isRunning = true;          //Start it up
    this->waitingTime = waitingTime; //Set the waiting time
    this->dir = dir;                 //Set the managed direction
    this->startTime = millis();
}

void MiniCountdown::stop()
{
    this->isRunning = false;
    this->startTime = -1;
    this->waitingTime = -1;
}

void MiniCountdown::reset()
{
    this->isRunning = false;
    this->hasFinished = false;
    this->waitingTime = -1;
    this->dir = Direction::NONE;
    this->startTime = -1;
}
/** Updates its status
* @return boolean which determines whether or not continue
*/
void MiniCountdown::update()
{
    uint16_t currentTime = millis();
    hasFinished = currentTime >= startTime + waitingTime;
}

bool MiniCountdown::getIsRunning()
{
    return this->isRunning;
}
bool MiniCountdown::getHasFinished()
{
    return this->hasFinished;
}
Direction MiniCountdown::getDirection()
{
    return this->dir;
}

void MiniCountdown::setStatus(bool value)
{
    this->isRunning = value;
}