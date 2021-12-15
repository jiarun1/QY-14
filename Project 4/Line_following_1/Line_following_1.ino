/*
 Name:    Line_following_1.ino
 Created: 2021/12/12 16:04:14
 Author:  LJR
*/

#include <MsTimer2.h>
#include <Wire.h>           // Include the Wire library for I2C communication
#include <LiquidCrystal.h>  // Include the Liquid Crystal library for driving the LCD

// PID Loop Constant analog inputs

#define ECD_READ_CLOCK      2
#define ECD_PIN_B           11

#define LEFT_SENSOR_PIN     A6
#define RIGHT_SENSOR_PIN    A7

#define MIDDLE_POSITION 0

#define PID_MAX_IOUT    20
#define PID_MAX_OUT     60

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

typedef struct
{
    float kp;
    float ki;
    float kd;
}PID_Param;

typedef struct
{
    PID_Param pid_param;

    float max_out;
    float max_iout;

    float error;
    float last_error;

    float Pout;
    float Iout;
    float Dout;
    float Out;
}PID;

typedef struct
{
    int left_sensor;
    int right_sensor;
}sensor_struct;

typedef struct
{
    int speed_1;
    bool dir;

}motor_struct;

motor_struct motor_control[4];

PID Line_pid;
PID_Param old_pid_param;
sensor_struct sensor_data;

int motor_speed[4];
const int motor_speed_init[4] = { 20,20,20,20 };
const int motor_position[4] = { 1,1,-1,-1 };//1 for left,-1 for right;

void setup()
{
    Wire.begin();        // Join the I2C bus as a master (address optional for master)
    Serial.begin(9600);  // Start serial for output to PC

    Input_init();
    PID_init(&Line_pid,0.1,0.40,0.1);//0.1,2.8,1);//0.4,1);

    Timer_init();

    pinMode(ECD_READ_CLOCK, INPUT_PULLUP);
    pinMode(ECD_PIN_B, INPUT_PULLUP);

}

void loop()
{
 
    float output = PID_calculate(&Line_pid, MIDDLE_POSITION, get_position()); // Calculate the PID output.

    for (int i = 0;i < 4;i++)
    {
        motor_speed[i] = motor_speed_init[i] + motor_position[i] * output;
    }
    decoding_motor(motor_speed);
    send_data_commend();
}

inline void Input_init(void)
{
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(RIGHT_SENSOR_PIN, INPUT);
}

float PID_calculate(PID* pid,float set,float now)
{
    pid->last_error = pid->error;
    pid->error = set - now;

    pid->Pout = pid->pid_param.kp * pid->error;
    pid->Iout += pid->pid_param.ki * pid->error;
    pid->Dout = pid->pid_param.kd * (pid->error - pid->last_error);

    LimitMax(pid->Iout, pid->max_iout);
    pid->Out = pid->Pout + pid->Iout + pid->Dout;
    LimitMax(pid->Out, pid->max_out);

    return pid->Out;
}

void PID_init(PID* pid,float kp,float ki,float kd)
{
    pid->pid_param.kp = kp;
    pid->pid_param.ki = ki;
    pid->pid_param.kd = kd;

    pid->max_iout = PID_MAX_IOUT;
    pid->max_out = PID_MAX_OUT;

    pid->last_error = pid->error = pid->Iout = pid->Dout = pid->Pout = pid->Out = 0;
}

inline void Timer_init(void)
{
    MsTimer2::set(1, get_sensor_data);
    MsTimer2::start();
}

void get_sensor_data(void)
{
    sensor_data.left_sensor = (analogRead(LEFT_SENSOR_PIN)-18)*2;
    sensor_data.right_sensor = (analogRead(RIGHT_SENSOR_PIN)-41);
}

float get_position(void)
{
    //TODO:use matlab to get a function which is the sensor data
    return (sensor_data.left_sensor - sensor_data.right_sensor);
}

void send_data_commend(void)
{
    Wire.beginTransmission(42);
    Wire.write("ba");
    for (int i = 0;i < 4;i++)
    {
        if (motor_control[i].dir == 0)
            Wire.write("r");
        else if (motor_control[i].dir == 1)
            Wire.write("f");
    }
    for (int i = 0;i < 4;i++)
    {
        Wire.write((byte)motor_control[i].speed_1);
        Wire.write(0);
    }
    Wire.endTransmission();
}

void decoding_motor(int motor_speed[4])
{
    for (int i = 0;i < 4;i++)
    {
        if (motor_speed[i] < 0)
        {
            motor_control[i].dir = 0;
        }
        else if (motor_speed[i] > 0)
        {
            motor_control[i].dir = 1;
        }
        motor_control[i].speed_1 = abs(motor_speed[i]);
    }
}