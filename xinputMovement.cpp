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

/** Choose movement for a single motion value.
 * @param motions 16 bit values of y axis motion
 * @return Currently managed motion. Could have value of zero if not managed.
 */
Motion XinputMovement::manageCurrentMotion(int16_t leftMotion, int16_t rightMotion)
{
    //! it's still overflowing!
    //TODO add upper bound
    bool isManageable;
    Direction chosenDirection;
    Motion currentMotion = Motion(0, Direction::NONE);
    int16_t leftAbsMotion = abs(leftMotion);
    int16_t rightAbsMotion = abs(rightMotion);
    chosenDirection = leftAbsMotion > rightAbsMotion ? Direction::LEFT : Direction::RIGHT;

    switch (chosenDirection)
    {
    case LEFT:
        isManageable = leftAbsMotion >= MIN_Y;
        break;
    case RIGHT:

        isManageable = rightAbsMotion >= MIN_Y;
        break;
    }

    if (isManageable)
    {

        //Set current motion
        currentMotion = Motion(chosenDirection == LEFT ? leftMotion : rightMotion, chosenDirection);

        if (prevMotion.getDirection() == chosenDirection)
            currentMotion.modifyValue(currentMotion.getAbsValue() > prevMotion.getAbsValue() ? INCREMENT : -DECREMENT);
    }
    

    return currentMotion;
}
/** Checks timers to force disable whatever mover is moving, to flip flop between the two.
 * @return Wheter or not an action was blocked. True if it was, false if it wasn't
 */
bool XinputMovement::checkTimers()
{
    bool isStopped = false;
    stopMotionCountdown.update(); //continues it

    //Currently running and same direction as current
    if (stopMotionCountdown.getIsRunning() && (stopMotionCountdown.getDirection() == managedMotion.getDirection()))
    {

        isStopped = false;
    }
    else if (stopMotionCountdown.getHasFinished() && (stopMotionCountdown.getDirection() == managedMotion.getDirection()))
    {

        //block current motion until it doesn't switch to other mover
        managedMotion.setValue(0); //Forces it to go to 0
        managedMotion.setDirection(Direction::NONE);

        isStopped = true;
    }

    return isStopped;
}

/** Based on what mover is actually registering a movement, it outputs a forward movement\
*/
void XinputMovement::moveForward()
{

    //needs a top speed and some sort of way to increment it in a decently smooth way
    //some sort of map?
    if (prevMotion.getDirection() != managedMotion.getDirection())
    {
        Serial.println("chck timers 1");

        stopMotionCountdown.stop();
        stopMotionCountdown.reset();
    }
    else if (!stopMotionCountdown.getIsRunning())
    {
        Serial.println("chck timers 2");

        stopMotionCountdown.start(COUNTDOWN_STOP_SINGLE_MOTION, managedMotion.getDirection());
    }

    if (prevMotion.getValue() != 0)
    {
        Serial.println("chck timers 3");

        XInput.setJoystickY(JOY_LEFT, managedMotion.getValue() * 2);
    }
}

//////////////////////////////////////////////////////
//PUBLIC METHODS
//////////////////////////////////////////////////////

void XinputMovement::manageMotions(int16_t leftMotion, int16_t rightMotion)
{
    //Saves old managedMotion to the right PrevMotion
    prevMotion.setValue(managedMotion.getValue());
    prevMotion.setDirection(managedMotion.getDirection());

    //Updates managedMotion
    managedMotion = manageCurrentMotion(leftMotion, rightMotion);
    if (!checkTimers())
    {
        moveForward();
    }
}