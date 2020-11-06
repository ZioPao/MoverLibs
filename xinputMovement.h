#include "Arduino.h"
#include "XInput.h"
#include "timerUtils.h"

#define MOVERS_AMOUNT 2 //How many movers are connected

#define MIN_Y 300
#define MIN_X 4000
#define INCREMENT 700 //How much to add when there is a greater motion than the old one
#define DECREMENT 400
#define COUNTDOWN_STOP 800 //0.8 sec to let it stop completely

/* To test stuff without using the XInput library*/
//#define DEBUG

class XinputMovement
{
private:
    enum direction
    {
        LEFT,
        RIGHT
    };
    bool isMovingForward; //Becomes true only when all the movers are in sync and moving
    bool isMovingLateral; //True only when tapping, moves in that direction

    MiniCountdown isMovingCountdown;

    int16_t motions[2] = {0, 0}; //Declares an array where our values are stored for the Xinput lib to use
    bool manageSingleMotion(int16_t motion, direction id);

public:
    XinputMovement();
    void manageMotions(int16_t motion1, int16_t motion2);
};
