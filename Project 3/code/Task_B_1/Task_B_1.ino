/*
	Name:       bicommunication.ino
	Created:	2021/12/4 23:35:24
	Author:     LAPTOP-MR5CQQ9V\LJR
*/

// Define User Types below here or use a .h file
//
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include <MsTimer2.h>

// Define Function Prototypes that use User Types below here or use a .h file
//
#define RF24_CHANNAL_TX		100

#define ONE_METER			1800

#define VEHICLE_CONTROL_PERIOD	10
#define VEHICLE_CONTROL_speed_1_CAL	 (100000/VEHICLE_CONTROL_PERIOD)

unsigned long last_encoder1Value = 0x80000000;
unsigned long encoder1Value = 0x80000000;
unsigned long last_encoder2Value = 0x80000000;
unsigned long encoder2Value = 0x80000000;

int speed_dir_1 = 0;
int speed_dir_2 = 0;

//typedef 
typedef struct
{
	struct
	{
		int motor_speed[4];
	}remote_tx;
	struct
	{
		bool state;
		int speed_1;
    int speed_2;
    long distance;
		//unsigned long distance;
	}car_tx;
}information;

typedef struct
{
	int speed_1;
	bool dir;

}motor_struct;

void transform(void);

//global variable
RF24 rf24(9, 10);
const byte address[][6] = { "1NODE", "2NODE" };
information info;
const byte pipe = 1;

long unsigned int global_time = 0;
volatile long unsigned int time_limit_tmp = 0;


/************************************************************/
motor_struct motor_control[4];

// The setup() function runs once each time the micro-controller starts
void setup()
{
	rf24.begin();
	rf24.setChannel(RF24_CHANNAL_TX);

	rf24.openWritingPipe(address[1]); // 00002
	rf24.openReadingPipe(pipe, address[0]); // 00001

	rf24.setPALevel(RF24_PA_MAX);
	rf24.setDataRate(RF24_2MBPS);
	Serial.begin(9600);


	Timer_init();
	Wire.begin();
	Serial.println("init success");

 info.car_tx.distance = 0;
 info.car_tx.speed_1 = 0;
  info.car_tx.speed_2 = 0;
 info.car_tx.state = 0;
  readEncoder1();
  readEncoder2();

}

void Timer_init(void)
{
	MsTimer2::set(VEHICLE_CONTROL_PERIOD, vehicle_control);
	MsTimer2::start();
}



void vehicle_control(void)
{
	interrupts();
	decoding_connection();
	send_data_commend();
	get_feedback_data();
}

void get_feedback_data(void)
{
	readEncoder1();
  readEncoder2();
	unsigned long tmp;
  unsigned long tmp_2;
  
  if(last_encoder1Value >= encoder1Value){tmp = (last_encoder1Value - encoder1Value);speed_dir_1 = -1;}
  else if(last_encoder1Value < encoder1Value){tmp = encoder1Value - last_encoder1Value;speed_dir_1 = 1;}
  if(tmp > 50)tmp = 0;

  if(last_encoder2Value >= encoder2Value){tmp_2 = (last_encoder2Value - encoder2Value);speed_dir_2 = -1;}
  else if(last_encoder2Value < encoder2Value){tmp_2 = encoder2Value - last_encoder2Value;speed_dir_2 = 1;}
  if(tmp > 50)tmp_2 = 0;
  /*
  Serial.print(encoder1Value);
  Serial.print('\t');
  Serial.print(last_encoder1Value);
  Serial.print('\t');
  Serial.print((int)tmp);
  */
  Serial.println(tmp);
	info.car_tx.distance += (long)(tmp *1000 / ONE_METER);
	info.car_tx.state = ((tmp < 10) ? 0 : 1);
	info.car_tx.speed_1 = speed_dir_1 * (int)(tmp *0.55);//VEHICLE_CONTROL_speed_1_CAL / ONE_METER);
  info.car_tx.speed_2 = speed_dir_2 * (int)(tmp_2 *0.55);
  //print_data_2();
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

void readEncoder2(void)
{
  long unsigned int encoder2 = 0;
  Wire.beginTransmission(42);
  Wire.write("i2");
  Wire.endTransmission();
  //delay(1);
  Wire.requestFrom(42, 8);
  //delay(4);
  //if(Wire.available()==8)
  {
    encoder2 = (long unsigned int) Wire.read();
    encoder2 += ((long unsigned int) Wire.read() << 8);
    encoder2 += ((long unsigned int) Wire.read() << 16);
    encoder2 += ((long unsigned int) Wire.read() << 24);
  }
  last_encoder2Value = encoder2Value;
  encoder2Value = encoder2;
  if (encoder2Value > 0x80040000)
  {
    encoder2Value = 0x80000000;
  }
  else if (encoder2Value < 0x7FFC0000)
  {
    encoder2Value = 0x80000000;
  }
}

void decoding_connection(void)
{
	for (int i = 0;i < 4;i++)
	{
		if (info.remote_tx.motor_speed[i] < 0)
		{
			motor_control[i].dir = 0;
		}
		else if (info.remote_tx.motor_speed[i] > 0)
		{
			motor_control[i].dir = 1;
		}
		motor_control[i].speed_1 = abs(info.remote_tx.motor_speed[i]);
	}
}

// Add the main program code into the continuous loop() function
void loop()
{
	transform();
}




void transform(void)
{
	if (receive((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx), 3))
	{
		//delay(5);//delay(5);
		send_data((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
		print_data();
	}
}

bool receive(void* data, uint8_t size1, uint8_t timelimit)
{
	bool st = false;
	rf24.startListening();
	if (rf24.available())
	{
		rf24.read((uint8_t*)data, size1);
		st = true;
	}
	return st;
}


void send_data(void* data, uint8_t size1)
{
	rf24.stopListening();
	delay(1);
	bool send_data_tmp = rf24.write((uint8_t*)data, (uint8_t)size1);
	//Serial.println(send_data_tmp);
	rf24.startListening();
}

void print_data(void)
{/*
	Serial.print("motor1:");
	Serial.print(info.remote_tx.motor_speed[0]);
	Serial.print("\tmotor2:");
	Serial.print(info.remote_tx.motor_speed[1]);
	Serial.print("\tmotor3:");
	Serial.print(info.remote_tx.motor_speed[2]);
	Serial.print("\tmotor4:");
	Serial.println(info.remote_tx.motor_speed[3]);*/
}

void print_data_2()
{
  Serial.print("distance:");
  Serial.print(info.car_tx.distance);
  Serial.print("\spteed:");
  Serial.print(info.car_tx.speed_1);
  Serial.print("\tstate:");
  Serial.println(info.car_tx.state);
}
