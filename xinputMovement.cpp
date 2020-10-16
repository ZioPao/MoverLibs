#include "xinputMovement.h"

#ifdef DEBUG
int16_t debugValue;
#endif

XinputMovement::XinputMovement()
{

    XInput.begin();
    isMoving = false;
}
//////////////////////////////////////////////////////
//PRIVATE METHODS
//////////////////////////////////////////////////////

/** Manage movement for a single motion value.
 * @param motion 16 bit value of y axis motion
 * @param id integer to design a single mover
 * @return True if managed, false if not manageable.
 */
bool XinputMovement::manageSingleMotion(int16_t motion, uint8_t id)
{
    bool isManageable;
    int16_t lastValue = motions[id]; //Last value to compare it to the newest one

    //! it's still overflowing!
    //TODO add upper bound

    //isMoving becomes true only when both of motions are managed
    if (abs(motion) >= MIN_Y)
    {
        isManageable = true;

        if (abs(motion) > abs(lastValue))
        {
            motions[id] = motions[id] + INCREMENT;
        }
        else
        {
            motions[id] = motions[id] - DECREMENT;
        }
    }

    else
    {
        isManageable = false;
    }

    lastValue = motions[id]; //Overwrite the new value
    return isManageable;
}

//////////////////////////////////////////////////////
//PUBLIC METHODS
//////////////////////////////////////////////////////

void XinputMovement::manageMotions(int16_t motion1, int16_t motion2)
{
    bool prevIsMoving = isMoving;
    //Using a single && line doesn't work, dunno why and I don't care tbh
    bool mot1, mot2;
    mot1 = manageSingleMotion(motion1, 0);
    mot2 = manageSingleMotion(motion2, 1);

    isMoving = mot1 && mot2;

    if (isMoving)
    {
        //if the countdown was active, it'll stop it forcefully
        isMovingCountdown.stop();

        int16_t tmpValue = 0;
        for (int x : motions)
        {
            tmpValue += x;
        }
        tmpValue *= 2;  //! Non ottimale, principalmente per test
        XInput.setJoystickY(JOY_LEFT, (uint16_t)tmpValue); 
    }
    else
    {
        /*
        *  If it stopped moving, starts a timer and check if it should forcefully stop or not,
        *  so it checks if the countdown is running while isMoving is false
        */
        if (isMovingCountdown.getStatus())
        {
            digitalWrite(17, LOW);
            XInput.setJoystickY(JOY_LEFT, 0);
            digitalWrite(17, HIGH);
        }
        else if (prevIsMoving)
        {
            //If it was moving before and now it stopped, start the timer

            isMovingCountdown.start(COUNTDOWN_STOP);
        }
    }
}
