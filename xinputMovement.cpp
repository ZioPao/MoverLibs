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
    else
    {
        return currentMotion;
    }
}
/** Checks timers to force disable whatever mover is moving, to flip flop between the two.
 * @return Wheter or not an action was blocked. True if it was, false if it wasn't
 */
bool XinputMovement::checkTimers()
{

    bool isStopped;
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

    if (prevMotion.getDirection() != managedMotion.getDirection())
    {
        stopMotionCountdown.stop();
        stopMotionCountdown.reset();
    }
    else if (!stopMotionCountdown.getIsRunning())
    {
        stopMotionCountdown.start(COUNTDOWN_STOP_SINGLE_MOTION, managedMotion.getDirection());
    }

    if (prevMotion.getValue() != 0)
    {

        XInput.setJoystickY(JOY_LEFT, managedMotion.getValue() * 2);
    }
}

/*
void XinputMovement::manageLateralMovement()
{
    isMovingCountdown.stop(); //if the countdown was active, it'll stop it forcefully

    Direction dir;
    //Keep tapping
    int16_t tmpValue = 0;

    //Check which mover is moving
    motionLeft ? dir = LEFT : dir = RIGHT;

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
*/
/*void XinputMovement::manageStopping()
{
   
    *  If   \ stopped moving, starts a timer and check if it should forcefully stop or not,
    *  so it checks if the countdown is running while isMoving is false
    
    if (isMovingCountdown.getStatus())
    {
        digitalWrite(17, LOW);
        XInput.setJoystickY(JOY_LEFT, 0);
        digitalWrite(17, HIGH);
    }
    else if (previousMotion == RIGHT || previousMotion == LEFT)
    {
        //If it was moving before and now it stopped, start the timer

        isMovingCountdown.start(COUNTDOWN_STOP);
    }
}*/

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

    if (checkTimers())
    {
        moveForward();
    }

    //use managed motions here

    //Manage everything else

    //check timers
    //if left currently ongoing (prevMotion = left and currentMotion = left and timer going), then just keep adding i guess?
    //if left currently ongoing but timer has finished, stop movmvent for left for 1 sec
    //if left is stopped or not moving anymore, wait for right

    //previous motion must be checked HERE. if left is moving and the countdown is on, then left will stay until right is active.
    // if right isn't activated, then previousMotion must be disabled

    // prevMotions[FORWARD] = isMovingForward;
    // prevMotions[GENERIC_LATERAL] = isMovingLateral;

    //Using a single && line doesn't work, dunno why and I don't care tbh

    //one motion must win. not both of them

    // motionLeft = manageSingleMotion(leftMotion, LEFT);
    // motionRight = manageSingleMotion(rightMotion, RIGHT);

    //isMovingForward = motionLeft && motionRight;        //that's just wrong
    //isMovingLateral = motionLeft ^ motionRight; //Only one has to be active

    /* Three possible type of 'inputs':
        1) Forward
        2) Lateral
        3) Jumping?
        4) Stopping
    */

    // if (isMovingForward)
    //     manageForwardMovement();
    // if (isMovingLateral)
    //     manageLateralMovement();
    // else
    //     manageStopping();
}