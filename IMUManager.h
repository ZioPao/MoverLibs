#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"


#define INTERRUPT_PIN 7

//#define IMU_DEBUG
class IMUManager
{
private:
    MPU6050 mpu = MPU6050(0x68);
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    // Values to get
    VectorFloat gravity;    // [x, y, z]            gravity vector
    Quaternion q;           // [w, x, y, z]         quaternion container
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements      

    void MPULoading();

public:

    void setup(int16_t acc_x_offset, int16_t acc_y_offset, int16_t acc_z_offset, int16_t gyr_x_offset, int16_t gyr_y_offset, int16_t gyr_z_offset);
    VectorInt16 getAccelValues();

    void setDmpReady(bool value);
};
    
    void dmpDataReady();
