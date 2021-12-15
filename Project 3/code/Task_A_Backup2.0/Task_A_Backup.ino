#include <SPI.h>
#include <RF24.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include <MsTimer2.h>

/*--------------      car related         -------------*/

#define ONE_METER (1800)
#define TRANSMIT_PERIOD (10)

/*---------------        RF24           ---------------*/

#define RF24_CHANNAL_TX		100

#define MIN_GRADE   5
#define GRADE_INCREASE_DISTANCE   1200//-800//-800   //180//ONE_METER/100*50  //(38.516+51.825)//<< the number increase of the grade

#define TASK_DELAY     delay(2)

enum mpu_axis {
    yaw_axis = 0,
    pitch_axis = 1,
    roll_axis = 2
};

/************************FOR TASK VARIABLE*************************/

typedef struct
{
    bool direction_1;
    int  speed_1;
}motor_control_struct;

motor_control_struct motor_condition[4];

long unsigned int encoder1Value = 0x80000000;
long unsigned int last_encoder1Value = 0x80000000;

long unsigned int distance_target = 0;
long unsigned int distance_set_begin = 0;
long unsigned int distance_reamin_target = 0;

int angle_target = 0;
int angle_set_begin = 0;

/*********************       FOR TASK Control    *****************/

int forward_speed[4] = { 52,52,50,50 };
int backward_speed[4] = { -50,-50,-50,-50 };
int left_speed[4] = { -50,-50,50,50 };
int right_speed[4] = { 70,70,-70,-70 };
int left_round_speed[4] = { 40,40,90,90 };
int right_round_speed[4] = { 70,70,50,50 };
int upward_speed[4] = {75,75,70,70};

/******************************************************************/

MPU6050 mpu;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float mpu_data[3];
int mpu_real_data[3];
int mpu_last_real_data[3];

const int16_t mpu_offset[6] = { -1646, 852, 1021, -41, 54, -14 };

bool angle_set_limit = false;

/************************        RF24     *************************/
typedef struct
{
    bool sta;
    int inclination;
    long distance;
}information;//<<transform data struct

information info;

RF24 rf24(9, 10);
const byte address[] = { "1NODE" };
const byte pipe = 1;


void setup() {
    Serial.begin(9600);
    Serial.println("Serial Init Success");
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    Serial.println("I2C Init Success");
    mpu_initialize();
    Serial.println("MPU Init Success");
    //Serial.println("Timer Init Success");
    rf24_init();
    Serial.println("RF24 Init Success");
    Timer_init();
    Serial.println("ok2");
}

void loop() 
{   
    go_straight(1, (long unsigned)((float)ONE_METER * 2.55));
    TASK_DELAY; 
    turn(1,30);
    TASK_DELAY;
    go_straight(1, (long unsigned)((float)ONE_METER * 1.38));//1.41421));
    TASK_DELAY;
    turn(1, 30);
    TASK_DELAY;
    go_straight(0, (long unsigned)((float)ONE_METER * 0.50));
    TASK_DELAY;
    turn(1, 42);
    TASK_DELAY;
    go_straight(1, (long unsigned)((float)ONE_METER * 1.0));//1.21803));
    TASK_DELAY;
    turn(1, 24);
    TASK_DELAY;
    go_straight(1, (long unsigned)((float)ONE_METER * 3.5));
    TASK_DELAY;
    turn(1, 30);
    TASK_DELAY;
    go_straight(1, (long unsigned)((float)ONE_METER * 0.707));//0.70710));
    TASK_DELAY;
    turn(1, 30);
    TASK_DELAY;
    turn_round(1, 82);
    TASK_DELAY;
    go_straight(1, (long unsigned)((float)ONE_METER * 0.54));
    while (1);
}

/*-----------------        TASK CONTROL FUNCTION       ---------------------*/

/**
* @brief    set_straight task in loop()
* @param    dir     :choose the direction
*               true:forward    ||  false:backward
* @param    target  :the distance;
* @usage    loop();
*/
inline void go_straight(bool dir,long unsigned int target)
{
    if (dir == true)vehicle_control(forward_speed);
    else if (dir == false)vehicle_control(backward_speed);
    set_distance_target(target);
    while (check_distance())
    {
        delay(5);//readEncoder1();
        if (check_pitch())uphill_task();
    }
    stop_vehicle();
    Serial.print("stop go straight of");
    Serial.println(target/ONE_METER);
}

/**
* @brief    set turn task in loop()
* @param    dir     :choose the direction
*               true:left    ||  false:right
* @param    target  :the angle;
* @usage    loop();
*/
inline void turn(bool dir, int angle)
{
    if (dir == true)vehicle_control(left_speed);
    else if (dir == false)vehicle_control(right_speed);
    Serial.println("ok10");
    set_angle_target(angle);
    Serial.println("ok11");
    while (check_angle())
    {
        delay(3);
        //mpu_get_data();
    }
    stop_vehicle();
    Serial.print("stop turn value of:");
    Serial.println(angle);
}

/**
* @brief    set turn roud task in loop()
* @param    dir     :choose the direction
*               true:left    ||  false:right
* @param    target  :the angle;
* @usage    loop();
*/
inline void turn_round(bool dir, int angle)
{
    if (dir == true)vehicle_control(left_round_speed);
    else if (dir == false)vehicle_control(right_round_speed);
    set_angle_target(angle);
    while (check_angle())
    {
        delay(1);
    }
    stop_vehicle();
    Serial.println("stop turn around");
}

/**
* @brief    used to set the uphill task
*/
void uphill_task(void)
{
    long unsigned int reamin_target = 0;

    if(encoder1Value > distance_set_begin) reamin_target = encoder1Value - distance_set_begin;
    else if (encoder1Value < distance_set_begin) reamin_target = distance_set_begin - encoder1Value;

    vehicle_control(upward_speed);
    Serial.println("into_task_uphill");
    while (check_pitch())delay(10);
    delay(300);
    stop_vehicle();
    delay(2000);


    turn_1(1,100);
    turn_1(1,50);
    turn_1(1,75);
    turn_1(1,80);
    /*angle_set_begin = mpu_data[yaw_axis];
    vehicle_control(left_speed);
    delay(300);
    while (check_square())delay(5);*/
    stop_vehicle();
    vehicle_control(forward_speed);
    set_distance_target(reamin_target+GRADE_INCREASE_DISTANCE);
}

inline void turn_1(bool dir, int angle)
{
    if (dir == true)vehicle_control(left_speed);
    else if (dir == false)vehicle_control(right_speed);
    Serial.println("ok10");
    set_angle_target(angle);
    Serial.println("ok11");
    while (check_angle())
    {
        delay(5);
    }
    //stop_vehicle();
    //Serial.print("stop turn value of:");
    //Serial.println(angle);
}


inline void stop_vehicle()
{
    Wire.beginTransmission(42);
    Wire.write("ha");
    Wire.endTransmission();
}

void communication_task(void)
{
    interrupts();
    readEncoder1();
    mpu_get_data();
    get_feedback_data();
    rf24_send_data((uint8_t*)&info, sizeof(info));
    //Serial.println(mpu_real_data[yaw_axis]);
}



/*--------------------         INIT FUNCTION            ---------------*/

void Timer_init(void)
{
    MsTimer2::set(TRANSMIT_PERIOD, communication_task);
    MsTimer2::start();
}

void rf24_init()
{
    rf24.begin();
    rf24.setChannel(RF24_CHANNAL_TX);
    rf24.openWritingPipe(address);
    rf24.setPALevel(RF24_PA_MAX);
    rf24.setDataRate(RF24_2MBPS);
    rf24.stopListening();
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


/*-------------     DATA GET AND DEAL FUNCTION        -------------*/

void get_feedback_data(void)
{
    unsigned long tmp;
    int speed_dir_1 = 0;

    if (last_encoder1Value >= encoder1Value) { tmp = (last_encoder1Value - encoder1Value);speed_dir_1 = -1; }
    else if (last_encoder1Value < encoder1Value) { tmp = encoder1Value - last_encoder1Value;speed_dir_1 = 1; }
    if (tmp > 50)tmp = 0;
    
    info.distance += (long)(tmp * 1000 / ONE_METER);
    info.sta = ((tmp < 10) ? 0 : 1);
    info.inclination = mpu_real_data[yaw_axis];//[pitch_axis];
}

void rf24_send_data(void* data, uint8_t size1)
{
    bool send_tmp = rf24.write((uint8_t*)data, (uint8_t)size1);
    Serial.println(send_tmp);
}

inline void mpu_get_data()
{
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(mpu_data, &q, &gravity);
        for (int i = 0;i < 2;i++)
        {
            mpu_last_real_data[i] = mpu_real_data[i];
            mpu_real_data[i] = 180 * mpu_data[i] / M_PI;
        }
    }
}

void readEncoder1(void)
{
    long unsigned int encoder1 = 0;
    Wire.beginTransmission(42);
    Wire.write("i1");
    Wire.endTransmission();
    delay(1);
    Wire.requestFrom(42, 8);
    delay(1);
    //if(Wire.available()==8)
    {
        encoder1 = (long unsigned int) Wire.read();
        encoder1 += ((long unsigned int) Wire.read() << 8);
        encoder1 += ((long unsigned int) Wire.read() << 16);
        encoder1 += ((long unsigned int) Wire.read() << 24);
    }
    last_encoder1Value = encoder1Value;
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



/*------------      VEHICLE CONTROL FUNCTION           -------------*/

void vehicle_control(int vehicle_speed[4])
{
    motor_decode(vehicle_speed);
    delay(1);
    send_data_commend();
}

inline void send_data_commend(void) 
{
  Wire.beginTransmission(42);
  Wire.write("ba");
  for (int i = 0;i < 4;i++)
  {
    if (motor_condition[i].direction_1 == 0)
      Wire.write("r");
    else if (motor_condition[i].direction_1 == 1)
      Wire.write("f");
  }
  for (int i = 0;i < 4;i++)
  {
    Wire.write((byte)motor_condition[i].speed_1);
    Wire.write(0);
  }
  Wire.endTransmission();
}

inline void motor_decode(int motor_speed[4])
{
    for (int i = 0;i < 4;i++)
    {
        if (motor_speed[i] >= 0) motor_condition[i].direction_1 = 1;
        else if (motor_speed[i] < 0)motor_condition[i].direction_1 = 0;

        motor_condition[i].speed_1 = abs(motor_speed[i]);
    }
}

//check if the target are reached

/**
* @brief
* @return       if target have been reached
* @retval       0   :   reached the target
* @retval       1   :   not reached the target
**/
bool check_distance(void)// if can keep running
{
    if (encoder1Value > (distance_set_begin + distance_target))return 0;
    else if (encoder1Value < (distance_set_begin - distance_target))return 0;

    return 1;
}

/**
* @brief
* @return       if target have been reached
* @retval       0   :   reached the target
* @retval       1   :   not reached the target
**/
bool check_angle()//if can run return 1
{
    if (mpu_real_data[yaw_axis] < set_angle_limit(angle_set_begin + angle_target)
        && mpu_real_data[yaw_axis] > set_angle_limit(angle_set_begin - angle_target))return 1;
    if (angle_set_limit == true)
    {
        if (mpu_real_data[yaw_axis] < set_angle_limit(angle_set_begin + angle_target)
            || mpu_real_data[yaw_axis] > set_angle_limit(angle_set_begin - angle_target))return 1;
        angle_set_limit = false;
    }

    return 0;
}

bool check_pitch(void)
{
    if (mpu_real_data[pitch_axis] > MIN_GRADE)
    {
        return 1;
    }
    return 0;
}

bool check_square(void)
{
    if (abs(mpu_real_data[yaw_axis] - angle_set_begin) < 8)
    {
        return 0;
    }

    return 1;
}

//set the target

void set_distance_target(long unsigned int target)
{
    //readEncoder1();
    distance_target = target;
    distance_set_begin = encoder1Value;
}

void set_angle_target(int target)
{
    angle_target = target;
    angle_set_begin = (mpu_real_data[pitch_axis]);
}

/*-------------      FUNCTION WITH CERTAIN USES    --------------*/

inline long unsigned int choose_min_value(long unsigned int X, long unsigned int Y)
{
    if (X > Y)return Y;
    else return X;
}

inline int set_angle_limit(int input)
{
    if (input > 180) { angle_set_limit = true;return (input - 360); }
    else if (input < -180) { angle_set_limit = true;return (input + 360); }
    return input;
}
