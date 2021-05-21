#include "IMUManager.h"

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady() {
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
    Wire.setWireTimeout(3000,true); 
	Wire.clearWireTimeoutFlag();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // initialize device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // Manage dmp
    uint8_t dmpStatus = mpu.dmpInitialize();

    // dmp correctly initialized
    if (dmpStatus == 0)
    {
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

        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
    else
    {
        Serial.print("Error initializing!!!");
    }
}

VectorInt16 IMUManager::getAccelValues()
{

    if (!dmpReady)
        return;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

            return aaReal;

        //gyr -> x = ypr[0];
        //gyr -> y = ypr[1];
        //gyr -> z = ypr[2];


        //mpu.getMotion6(&acc->x, &acc->y, &acc->z, &gyr->x, &gyr->y, &gyr->z);
    }
}

void IMUManager::setDmpReady(bool value)
{
    this->dmpReady = value;
}
