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
 * @return True if managed, false if not manageable. Defines isMoving value.
 */
bool XinputMovement::manageSingleMotion(int16_t motion, uint8_t id)
{

    //Initial check

    //isMoving becomes true only when both of motions are executed and true

    if (abs(motion) >= MIN_Y)
    {

        int16_t lastValue = motions[id]; //Last value to compare it to the newest one

        //  Three possible cases

        /* 1) Currently moving, and new motions is bigger than the last one*/
        if (isMoving && abs(motion) > abs(lastValue))
        {
#ifdef DEBUG
            Serial.println("1° case, changed value");
#endif
            motions[id] = motions[id] + INCREMENT; //Saves the new value
        }
        /* 2) Not moving currently, so we need to "force it" at first*/
        else if (!isMoving)
        {
            motions[id] = MIN_Y;
        }
        /* 3) Movement should be stopped */
        else
        {
            motions[id] = 0;
        }

#ifndef DEBUG
        if (isMoving)
        {
            int16_t tmpValue = 0;

            for (int x : motions)
            {
                tmpValue += x;
            }
            XInput.setJoystickY(JOY_LEFT, tmpValue * 4); //Incrementa velocità
        }

#endif

#ifdef DEBUG
        Serial.print("Current activated motion -> ");
        Serial.print(motions[id]);
        Serial.print("  ");
        Serial.print(id);
        Serial.println();
#endif
        lastValue = motions[id]; //Overwrite the new value
        return true;
    }
    else
    {
        return false;
    }
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

    /*
     *  If it stopped moving, starts a timer and check if it should forcefully stop or not,
     *  so it checks if the countdown is running while isMoving is false
     */
    if (isMovingCountdown.getStatus() && !isMoving)
    {
        //when it reaches 0, it stops all the movements
        if (isMovingCountdown.update())
        {

            XInput.setJoystickY(JOY_LEFT, 0);
        }
    }
    else if (!isMoving && prevIsMoving)
    {
        //If it was moving before and now it stopped, start the timer

        isMovingCountdown.start(COUNTDOWN_STOP);
    }
}
