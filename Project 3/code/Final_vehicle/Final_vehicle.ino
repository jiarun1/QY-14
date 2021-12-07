/*
 Name:		Final_vehicle.ino
 Created:	2021/12/6 10:00:39
 Author:	LJR
*/

// Define User Types below here or use a .h file
//
#include <SPI.h>
#include "RF24.h"
#include <MsTimer2.h>

// Define Function Prototypes that use User Types below here or use a .h file
//
#define RF24_CHANNAL_TX		100
#define MODE 2

//typedef 
typedef struct
{
	struct
	{
		int left_right;
		int forw_back;
		bool sta;//run: 1 || stop :0
		bool mode;//circles: 1|| moving turn:0;
	}remote_tx;
	struct
	{
		int cur_speed;
		int cur_dir;
		bool cur_state;
	}car_tx;

}information;

int motor_speed[4] = { 0 };

// Define Functions below here or use other .ino or cpp files
//
void transform(void);

//global variable
RF24 rf24(9, 10);
const byte address[][6] = { (byte)"1NODE", (byte)"2NODE" };
information info;
uint8_t pipe;

long unsigned int global_time = 0;
volatile long unsigned int time_limit_tmp = 0;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	rf24.begin();
	rf24.openWritingPipe(address[0]); // 00002
	rf24.openReadingPipe(pipe, address[1]); // 00001
	rf24.setChannel(RF24_CHANNAL_TX);
	rf24.setPALevel(RF24_PA_HIGH);
	Serial.begin(9600);

	MsTimer2::set(1, Timer2ISR);
	Serial.println("init success");
	MsTimer2::start();
}

void Timer2ISR(void)
{
	global_time++;
}

// Add the main program code into the continuous loop() function
void loop()
{
#if (MODE==1)

	transform();
#elif (MODE==2)

	transform();
	Serial.println("LF:");
	Serial.println(info.remote_tx.left_right);
	Serial.println("FB:");
	Serial.println(info.remote_tx.left_right);
	Serial.println("MODE:");
	Serial.println(info.remote_tx.left_right);
	Serial.println("STA:");
	Serial.println(info.remote_tx.left_right);
	Serial.println("\n");
#endif

}


void transform(void)
{
#if (MODE==1)
	//rf24.stopListening();
	//rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
	send((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
	receive((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx), 2);

	//rf24.startListening();
	//time_limit_tmp = global_time;
	//while ((global_time - time_limit_tmp) <= 2)
	//{
	//	if (rf24.available(&pipe))
	//	{
	//		rf24.read((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
	//		break;
	//	}
	//}
	//rf24.stopListening();

#elif (MODE==2)
	//rf24.startListening();
	//delay(1);
	//rf24.read((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
	//rf24.stopListening();
	//rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));

	if (receive_num((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx), 1))
	{
		send((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
	}
#endif
}

bool receive_num(void* data, uint8_t size, uint8_t maxnum)
{
	bool st = false;
	while (maxnum)
	{
		if (rf24.available(&pipe))
		{
			rf24.read((uint8_t*)data, size);
			st = true;
			break;
		}
		maxnum--;
	}
	rf24.stopListening();
	return st;
}

bool receive(void* data, uint8_t size, uint8_t timelimit)
{
	bool st = false;
	rf24.startListening();
	time_limit_tmp = global_time;
	while ((global_time - time_limit_tmp) <= timelimit)
	{
		if (rf24.available(&pipe))
		{
			rf24.read((uint8_t*)data, size);
			st = true;
			break;
		}
	}
	rf24.stopListening();
	return st;
}

void send(void* data, uint8_t size)
{
	rf24.stopListening();
	rf24.write((uint8_t*)data, size);
}


/*
void transform(void)
{
#if (MODE==1)
	rf24.stopListening();
	rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
	rf24.startListening();
	delay(1);
	rf24.read((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
	rf24.stopListening();

#elif (MODE==2)
	rf24.startListening();
	delay(1);
	rf24.read((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
	rf24.stopListening();
	rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
#endif
}*/

void speed_calculate()
{


}