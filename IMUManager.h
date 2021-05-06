#include "I2Cdev.h"
#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#include "MPU6050.h"
#include "Wire.h"

#define INTERRUPT_PIN 7

//#define IMU_DEBUG

void dmpDataReady();

class IMUManager
{
private:
    MPU6050 mpu;
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    //test stuff
    int16_t ax, ay, az, gx, gy, gz;


    void MPULoading();

public:

    void setup(int16_t acc_x_offset, int16_t acc_y_offset, int16_t acc_z_offset, int16_t gyr_x_offset, int16_t gyr_y_offset, int16_t gyr_z_offset);
    void getGyroAndAccelValues(VectorInt16 *acc, VectorInt16 *gyr);
};
