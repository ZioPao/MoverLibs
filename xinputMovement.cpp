#include "xinputMovement.h"

#ifdef DEBUG
int16_t debugValue;
#endif

XinputMovement::XinputMovement()
{

    XInput.begin();
    isMovingForward = false;
    isMovingLateral = false;
}
//////////////////////////////////////////////////////
//PRIVATE METHODS
//////////////////////////////////////////////////////

/** Manage movement for a single motion value.
 * @param motion 16 bit value of y axis motion
 * @param direction enum to design a single mover, left or rigth
 * @return True if managed, false if not manageable.
 */
bool XinputMovement::manageSingleMotion(int16_t motion, Direction id)
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

    lastValue = motions[(int16_t)id]; //Overwrite the new value
    return isManageable;
}

void XinputMovement::manageForwardMovement()
{
    bool prevIsMoving = isMovingForward;
    isMovingCountdown.stop();        //if the countdown was active, it'll stop it forcefully
    int16_t tmpValue = 0;

    for (int x : motions)
    {
        tmpValue += x;
    }

    tmpValue *= 2; //! Non ottimale, principalmente per test
    XInput.setJoystickY(JOY_LEFT, (uint16_t)tmpValue);
}

void XinputMovement::manageLateralMovement(bool motion1)
{
    isMovingCountdown.stop();   //if the countdown was active, it'll stop it forcefully

    Direction dir;
    //Keep tapping
    int16_t tmpValue = 0;

    //Check which mover is moving
    motion1 ? dir = LEFT : dir = RIGHT;

    //Set the correct tmpValue
    dir == LEFT ? tmpValue -= motions[dir] : tmpValue += motions[dir];

    tmpValue *= 2; //! SOLO PER TEST!!!!
    XInput.setJoystickX(JOY_LEFT, (uint16_t)tmpValue);

#ifdef DEBUG
    Serial.print("Moving in this direction -> ");
    if (dir == LEFT)
    {
        Serial.print("Left");
    }
    else
    {
        Serial.print("Right");
    }
    Serial.println();
#endif
}

void XinputMovement::manageStopping(bool prevIsMovingForward){
    /*
    *  If   \ stopped moving, starts a timer and check if it should forcefully stop or not,
    *  so it checks if the countdown is running while isMoving is false
    */
    if (isMovingCountdown.getStatus())
    {
        digitalWrite(17, LOW);
        XInput.setJoystickY(JOY_LEFT, 0);
        digitalWrite(17, HIGH);
    }
    else if (prevIsMovingForward)
    {
        //If it was moving before and now it stopped, start the timer

        isMovingCountdown.start(COUNTDOWN_STOP);
    }
}

//////////////////////////////////////////////////////
//PUBLIC METHODS
//////////////////////////////////////////////////////

void XinputMovement::manageMotions(int16_t leftMotion, int16_t rightMotion)
{
    bool prevIsMovingForward = isMovingForward;
    bool prevIsMovingLateral = isMovingLateral;

    //Using a single && line doesn't work, dunno why and I don't care tbh
    bool mot1, mot2;

    mot1 = manageSingleMotion(leftMotion, LEFT);
    mot2 = manageSingleMotion(rightMotion, RIGHT);

    isMovingForward = mot1 && mot2;
    isMovingLateral = mot1 ^ mot2; //Only one has to be active

    /* Three possible type of 'inputs':
        1) Forward
        2) Lateral
        3) Jumping?
        4) Stopping
    */

    if (isMovingForward)
        manageForwardMovement();
    if (isMovingLateral)
        manageLateralMovement(mot1);
    else
        manageStopping(prevIsMovingForward);
}
