#include "MiniTimer.h"

void MiniTimer::setup(uint16_t interval)
{
    this->interval = interval;
    this->status = false;
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


