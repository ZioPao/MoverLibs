#include "IMUManager.h"

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high

void dmpDataReady()
{
    mpuInterrupt = true;
}

// ================================================================
// ===                         Setup                            ===
// ================================================================
void IMUManager::setup(int16_t acc_x_offset, int16_t acc_y_offset, int16_t acc_z_offset, int16_t gyr_x_offset, int16_t gyr_y_offset, int16_t gyr_z_offset)
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


    // offsets
    mpu.setXAccelOffset(acc_x_offset);
    mpu.setYAccelOffset(acc_y_offset);
    mpu.setZAccelOffset(acc_z_offset);
    
    mpu.setXGyroOffset(gyr_x_offset);
    mpu.setYGyroOffset(gyr_y_offset);
    mpu.setZGyroOffset(gyr_z_offset);

    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    
}


void IMUManager::getGyroAndAccelValues(VectorInt16* acc, VectorInt16* gyr){
    mpu.getMotion6(&acc -> x, &acc -> y, &acc -> z, &gyr -> x, &gyr -> y, &gyr -> z);
}

