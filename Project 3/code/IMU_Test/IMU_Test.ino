/*
 Name:		IMU_Test.ino
 Created:	2021/12/5 23:33:34
 Author:	LJR
*/


#include <MsTimer2.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"


#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

MPU6050 mpu;


// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

float mpu_data[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//           offset:            Ax,Ay,Az,gx,gy,gz
const int16_t mpu_offset[6] = { -1646, 852, 1028, -48, 64, -18 };


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady() {
    mpuInterrupt = true;
}

long unsigned int time_period = 0;


void setup() {

    Serial.begin(9600);
    Serial.println("ok1");
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    mpu.dmpInitialize();

    mpu_set_offset();

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
 
    // enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    Serial.println("ok");
    MsTimer2::set(2, timer2_IQR);
    MsTimer2::start();
}

void timer2_IQR(void)
{
    time_period++;
}


void loop() {
        mpu_get_data();
        Serial.print("time\t");
        Serial.print(time_period);
        Serial.print("\t");
        Serial.print("data\t");
        Serial.print(mpu_data[0] * 180 / M_PI);
        Serial.print("\t");
        Serial.print(mpu_data[1] * 180 / M_PI);
        Serial.print("\t");
        Serial.println(mpu_data[2] * 180 / M_PI);
}

inline void mpu_set_offset(void)
{
    mpu.setXAccelOffset(mpu_offset[1]);
    mpu.setYAccelOffset(mpu_offset[2]);
    mpu.setZAccelOffset(mpu_offset[3]);
    mpu.setXGyroOffset(mpu_offset[4]);
    mpu.setYGyroOffset(mpu_offset[5]);
    mpu.setZGyroOffset(mpu_offset[6]);
}

inline void mpu_get_data(void)
{
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) 
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(mpu_data, &q, &gravity);
    }
}
