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

        //no reason to have multiple managedMotions
        for (Motion prevMotion : prevMotions)
        {
            //probably slower, but I need clarity at this point
            if (prevMotion.getDirection() == chosenDirection)
            {
                currentMotion.modifyValue(currentMotion.getAbsValue() > prevMotion.getAbsValue() ? INCREMENT : -DECREMENT);
            }
        }
    }
    else
    {
        return currentMotion;
    }
}

void XinputMovement::checkTimers(Motion currentMotion)
{
    stopMotionCountdown.update(); //continues it

    //Currently running and same direction as current
    if (stopMotionCountdown.getIsRunning() && (stopMotionCountdown.getDirection() == currentMotion.getDirection())){
        //do nada
    }
    else if (stopMotionCountdown.getHasFinished() && (stopMotionCountdown.getDirection() == currentMotion.getDirection())){

        //block current motion until it doesn't switch to other mover
    }
   

    //check which mover, check corrisponding timer
}

/** Based on what mover is actually registering a movement, it outputs a forward movement\
 * 
 * 
*/
void XinputMovement::manageForwardMovement()
{

    //Starting movement

    //LEFt or RIGHT get registered for a certain amount of time. Let's say 1 full second.
    // After that, if the current motion isn't the opposite, it's basically a full stop

    //if last mov was left, and countdown has finished... then no more left, so activeMov must be disabled

    for (int dir = LEFT; dir < GENERIC_LATERAL; dir++)
    {

        //must check if was active anyway... A generic boolean?
        //if it was moving on the left, and the countdown has ended.... then stop movement and negates LEFT movement for a full second?
        if (prevMotions[dir])
        {

            if (currentMotionCountdown.getStatus())
            {
            }
            else
            {
                stopMotionCountdown.start(1);
                stoppedMovement = (Direction)dir;
            }
        }

        if (prevMotions[dir] && !currentMotionCountdown.getStatus())
        {
        }

        //Need to start the timer for the currentMotion
        if (prevMotions[dir] && currentMotionCountdown.getStatus())
        {
            currentMotionCountdown.start(1); //? sec or ms?
        }
    }

    if ((prevMotions[LEFT] && prevMotions[RIGHT]) == false)
    {
        isMovingCountdown.stop(); //if the countdown was active, it'll stop it forcefully
        int16_t tmpValue = 0;

        for (int x : motions)
        {
            tmpValue += x;
        }

        tmpValue *= 2; //! Non ottimale, principalmente per test
        XInput.setJoystickY(JOY_LEFT, (uint16_t)tmpValue);
    }
}

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

void XinputMovement::manageStopping()
{
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
    else if (previousMotion == RIGHT || previousMotion == LEFT)
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
    //Previous managed mover
    prevMotions[LEFT] = motionLeft;
    prevMotions[RIGHT] = motionRight;

    managedMotion = manageCurrentMotion(leftMotion, rightMotion);

    if (managedMotion.getValue() != 0)
    {

        //use managed motions here

        //Manage everything else

        //check timers
        //if left currently ongoing (prevMotion = left and currentMotion = left and timer going), then just keep adding i guess?
        //if left currently ongoing but timer has finished, stop movmvent for left for 1 sec
        //if left is stopped or not moving anymore, wait for right
    }

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