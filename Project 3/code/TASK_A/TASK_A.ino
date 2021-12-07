#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include <MsTimer2.h>
#include "LiquidCrystal.h"

#define MAX_MIN_LIMIT(NOW,MIN,MAX)  \
        {                           \
          if((*NOW)>(MAX))(*NOW)=(MAX);       \
          if((*NOW)<(MIN))(*NOW)=(MIN);       \
        }  

/************************car related******************/
#define ONE_METER (1800)
/******************************************************/

enum {
    angle_control = 0,
    distance_control = 1
};
/************************FOR TASK VARIABLE*************************/
long unsigned int encoder1Value = 0;
long unsigned int encoder2Value = 0;
long unsigned int encoder3Value = 0;
long unsigned int encoder4Value = 0;

int turn_value1 = 543;
int turn_value2 = 475;

int motor_speed_set[] = { 70,70,70,70 };

int motor_offset[4] = { 0,0,-4,-5 };
int motor_offset_A[4] = { 0,-2,0,-2 };

long unsigned int distance_target = 0;
float angle_target = 0;

long unsigned int distance_set_begin = 0;
float mpu_get_angle_begin = 0;

bool task_can_change = true;
bool control_mode = angle_control;//0 for angle control
bool vehicle_state = 0;
int task_num = 1;


/******************************************************************/

MPU6050 mpu;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float mpu_data[3];

const int16_t mpu_offset[6] = { -1646, 852, 1028, -48, 64, -18 };



void setup() {
    Serial.begin(9600);
    //mpu_initialize();
    Serial.println("ok1");
    Wire.begin();
    Serial.println("ok2");
    Timer_init();

}

void loop() {
}

void Timer_init(void)
{
    MsTimer2::set(10, vehicle_control);
    MsTimer2::start();
}

void vehicle_control(void)
{
    task_choose();
    readEncoder1();
    //mpu_get_data();
    
    if (control_mode == angle_control)
    {
        vehicle_state = check_angle();
    }
    else if (control_mode == distance_control)
    {
        vehicle_state = check_distance();
    }

    if (vehicle_state == 0)stop_vehicle();


}

void mpu_initialize()
{
    mpu.initialize();
    mpu.dmpInitialize();
    mpu_set_offset();
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
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

inline void mpu_get_data()
{
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(mpu_data, &q, &gravity);
    }
}

inline void stop_vehicle()
{
    Wire.beginTransmission(42);
    Wire.write("ha");
    Wire.endTransmission();
    task_can_change = true;
    task_num++;
}

bool check_distance(void)// if can keep running
{
    if (encoder1Value > distance_set_begin + distance_target)return 0;
    else if (encoder1Value < distance_set_begin - distance_target)return 0;
    
    return 1;
}

bool check_angle()//if can run return 1
{
    if (mpu_data[1] > set_angle_limit(mpu_get_angle_begin + angle_target))return 0;
    else if (mpu_data[1] < set_angle_limit(mpu_get_angle_begin - angle_target))return 0;

    return 1;


}

void readEncoder1(void)
{
    long unsigned int encoder1 = 0;
    Wire.beginTransmission(42);
    Wire.write("i1");
    Wire.endTransmission();
    //delay(1);
    Wire.requestFrom(42, 8);
    //delay(4);
    //if(Wire.available()==8)
    {
        encoder1 = (long unsigned int) Wire.read();
        encoder1 += ((long unsigned int) Wire.read() << 8);
        encoder1 += ((long unsigned int) Wire.read() << 16);
        encoder1 += ((long unsigned int) Wire.read() << 24);
    }
    encoder1Value = encoder1;
    if (encoder1Value > 0x80040000)
    {
        encoder1Value = 0x80000000;
    }
    else if (encoder1Value < 0x7FFC0000)
    {
        encoder1Value = 0x80000000;
    }
}

inline float set_angle_limit(float input)
{
    if (input > 360)return (input - 360);
    else if (input < 0)return (input + 360);
}

void turn(bool direction_1,int vehicle_speed[4],int angle)//right 1 +;left 0 -;
{
    task_can_change = false;
    control_mode = angle_control;
    mpu_get_angle_begin = mpu_data[1];
    float mpu_pitch_target = mpu_data[1];
    Serial.println("ok5");
    Wire.beginTransmission(42);
    Wire.write("ba");
    if (direction_1 == 0)
    {
        Wire.write("rrff");
    }
    else if (direction_1 == 1)
    {
        Wire.write("ffrr");
    }
    for (int i = 0;i <= 3;i++)
    {
        Wire.write(vehicle_speed[i]);
        Wire.write(0);
    }
    Wire.endTransmission();
    Serial.println("ok4");
}

void go_straight(bool direction_1,int vehicle_speed[4],float distance)
{
    //noInterrupts();
    task_can_change = false;
    control_mode = distance_control;
    distance_set_begin = encoder1Value;
    Wire.beginTransmission(42);
    if (direction_1 == 0)
    {
        Wire.write("baffff");
    }
    else if (direction_1 == 1)
    {
        Wire.write("barrrr");
    }
    for (int i = 0;i <= 3;i++)
    {
        Wire.write(vehicle_speed[i]);
        Wire.write(0);
    }
    Wire.endTransmission();
    //interrupts();
}

void task_choose(void)
{
    if (task_can_change == false)return;
    switch (task_num)
    {
    case '1':go_straight(0,motor_speed_set,1000);break;
    case '2':
    case '3':
    default:stop_vehicle();break;

    }
}
