#include "Arduino.h"
#include "XInput.h"

#define MOVERS_AMOUNT 2 //How many movers are connected

#define MIN_Y 300
#define START_MOVEMENT = MIN_Y
#define MIN_X 4000
#define INCREMENT 500 //How much to add when there is a greater motion than the old one


/* To test stuff without using the XInput library*/
//#define DEBUG

class XinputMovement
{
private:
    bool isMoving; //Becomes true only when all the movers are in sync and moving

    int16_t motions[2] = {0, 0}; //Declares an array where our values are stored for the Xinput lib to use
    bool manageSingleMotion(int16_t motion, uint8_t id);

public:

    XinputMovement();
    void manageMotions(int16_t motion1, int16_t motion2);
};
