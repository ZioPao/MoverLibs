#include "TimerUtils.h"

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

void MiniCountdown::start(uint16_t waitingTime)
{
    this->isRunning = true;          //Start it up
    this->waitingTime = waitingTime; //Set the waiting time
    this->startTime = millis();
}

void MiniCountdown::stop(){
    this->isRunning = false;
    this->startTime = -1;
    this -> waitingTime = -1;
}

/** Updates its status
* @return boolean which determines whether or not continue
*/
bool MiniCountdown::update()
{
    uint16_t currentTime = millis();

    if (currentTime >= startTime + waitingTime)
    {
        isRunning = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool MiniCountdown::getStatus()
{
    return this->isRunning;
}

void MiniCountdown::setStatus(bool value){
    this->isRunning = value;
}