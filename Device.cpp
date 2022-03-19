#include "Device.h"
#include <stdlib.h>
#include <iostream>

void sr04_c::Init(int echo, int trig)
{
	echo_pin = echo;
	trig_pin = trig;
	pinMode(echo_pin, INPUT);
	pinMode(trig_pin, OUTPUT);
	pullUpDnControl(echo_pin, PUD_DOWN);
}

float sr04_c::Read(int time_out)
{
	digitalWrite(trig_pin, HIGH);
	delayMicroseconds(20);
	digitalWrite(trig_pin, LOW);
	start_time = micros();
	while (digitalRead(echo_pin) == LOW)
	{
		if (abs(micros() - start_time) >= time_out)
		{
			return -1;
		}
	}
	start_time = micros();
	while (digitalRead(echo_pin) == HIGH)
	{
		if (abs(micros() - start_time) >= time_out)
		{
			return -1;
		}
	}
	distance = (float)(micros() - start_time) * 340.0f * 100.0f / 2.0f / 1000000.0f;
	return distance;
}










void vehicle_c::MotorDecode(int num, int speed)
{
	if (speed < 0)
	{
		motor[num].dir = BACKWARD;
		motor[num].speed = -speed;
	}
	else
	{
		motor[num].dir = FORWARD;
		motor[num].speed = speed;
	}
}

void vehicle_c::MotorSendCommand()
{
	serialPrintf(vehicle_port, "#Ba%c%c%c%c", 
							motor[0].dir,
							motor[1].dir,
							motor[2].dir,
							motor[3].dir);

	serialPrintf(vehicle_port, "%d %d %d %d", 
							motor[0].speed,
							motor[1].speed,
							motor[2].speed,
							motor[3].speed);
}


uint8_t vehicle_c::Init(const char* port,int baud_rate)
{
	vehicle_port = serialOpen(port, baud_rate);
	if (vehicle_port == -1)
	{
		return PORT_OPEN_ERR;
	}
	for (int i = 0;i < 4;i++)
	{
		motor[i].dir = FORWARD;
		motor[i].speed = 0;
		motor[i].position = (motor_pos_e)i;
	}
}

uint8_t vehicle_c::SetPosition(motor_pos_e motor1, motor_pos_e motor2, motor_pos_e motor3, motor_pos_e motor4)
{
	unsigned char position_judge = 0;
	position_judge |= 1 << (int)motor1;
	position_judge |= 1 << (int)motor2;
	position_judge |= 1 << (int)motor3;
	position_judge |= 1 << (int)motor4;
	if (position_judge != 15)
	{
		return POSITION_REPEAT;
	}

	motor[0].position = motor1;
	motor[1].position = motor2;
	motor[2].position = motor3;
	motor[3].position = motor4;
}

void vehicle_c::SetSpeed(int motor_all)
{
	for (int i = 0; i < 4; i++)
	{
		MotorDecode(i, motor_all);
	}
	MotorSendCommand();
}

void vehicle_c::SetSpeed(int motor_l, int motor_r)
{
	for (int i = 0; i < 4;i++)
	{
		if ((motor[i].position == LeftForward) || (motor[i].position == LeftBackward))
		{
			MotorDecode(i, motor_l);
		}
		else
		{
			MotorDecode(i, motor_r);
		}
	}
	MotorSendCommand();
}

void vehicle_c::SetSpeed(int motor1, int motor2,int motor3,int motor4)
{
	MotorDecode(0, motor1);
	MotorDecode(1, motor2);
	MotorDecode(2, motor3);
	MotorDecode(3, motor4);
	MotorSendCommand();
}