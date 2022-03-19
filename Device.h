#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include "wiringPi.h"
#include "wiringSerial.h"

class sr04_c
{
private:
	int echo_pin;
	int trig_pin;
	unsigned int start_time;
	float distance;

public:
	void Init(int echo, int trig);
	float Read(int time_out = 10000);
};

class vehicle_c
{
public:
	enum error_code_e
	{
		OK = 0,
		PORT_OPEN_ERR = 1,
		POSITION_REPEAT = 2,
	};
	enum motor_pos_e
	{
		LeftForward = 0,
		LeftBackward = 1,
		RightForward = 2,
		RightBackward = 3
	};

	uint8_t Init(const char* port, int baud_rate);
	uint8_t SetPosition(motor_pos_e motor1, motor_pos_e motor2, motor_pos_e motor3, motor_pos_e motor4);
	void SetSpeed(int motor_all);
	void SetSpeed(int motor_l, int motor_r);
	void SetSpeed(int motor1, int motor2, int motor3, int motor4);

private:
	enum motor_dir_e
	{
		FORWARD		= 'f',
		BACKWARD	= 'r'
	};

	struct motor_t
	{
		motor_dir_e dir;
		int speed;
		motor_pos_e position;
	};

	int vehicle_port;
	motor_t motor[4];

	void MotorDecode(int num, int speed);
	void MotorSendCommand();
};



#endif