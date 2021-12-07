/*
	Name:       bicommunication.ino
	Created:	2021/12/4 23:35:24
	Author:     LAPTOP-MR5CQQ9V\LJR
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
		uint8_t sta;//run: 1 || stop :0
		uint8_t mode;//circles: 1|| moving turn:0;
	}remote_tx;
	struct
	{
		int cur_speed;
		int cur_dir;
		uint8_t cur_state;
	}car_tx;
}information;

void transform(void);

//global variable
RF24 rf24(9, 10);
const byte address[][6] = { "1NODE", "2NODE" };
information info;
const byte pipe = 1;

long unsigned int global_time = 0;
volatile long unsigned int time_limit_tmp = 0;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	rf24.begin();
  rf24.setChannel(RF24_CHANNAL_TX);
#if (MODE==1)
	rf24.openWritingPipe(address[0]); // 00002
	rf24.openReadingPipe(pipe, address[1]); // 00001

#elif (MODE==2)
  rf24.openWritingPipe(address[1]); // 00002
  rf24.openReadingPipe(pipe, address[0]); // 00001
#endif
	rf24.setPALevel(RF24_PA_MAX);
	Serial.begin(9600);

	MsTimer2::set(10, Timer2ISR);
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

info.remote_tx.left_right = 70;
info.remote_tx.forw_back = 230;
info.remote_tx.sta = 1;
info.remote_tx.mode = 0;


	transform();
  delay(100);
#elif (MODE==2)

info.car_tx.cur_speed = 70;
info.car_tx.cur_dir = 1;
info.car_tx.cur_state = 0;

	transform();
  
#endif

}


void transform(void)
{
#if (MODE==1)
	//rf24.stopListening();
	//rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
	send((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
 
	receive((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx), 2);

  print_data_2();

 
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

	if (receive((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx), 3))
	{
    delay(1);
		send((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx));
    print_data();
	}
#endif
}

bool receive_num(void* data, uint8_t size1, uint8_t maxnum)
{
	bool st = false;
  rf24.startListening();
	while (maxnum)
	{
		if (rf24.available(&pipe))
		{
			rf24.read((uint8_t*)data, size1);
			st = true;
			break;
		}
		maxnum--;
	}
	rf24.stopListening();
	return st;
}

bool receive(void* data, uint8_t size1, uint8_t timelimit)
{
	bool st = false;
	rf24.startListening();
	//delay(1);
	//time_limit_tmp = global_time;
	//while ((global_time - time_limit_tmp) <= timelimit)
	{
		if (rf24.available(&pipe))
		{
			rf24.read((uint8_t*)data, size1);
			st = true;
			//break;
      
		}
	}
	//rf24.stopListening();
	return st;
}


void send(void* data, uint8_t size1)
{
  rf24.stopListening();
  delay(1);
	bool send_tmp = rf24.write((uint8_t*)data, (uint8_t)size1);
  Serial.println(send_tmp);
  rf24.startListening();
}

void print_data()
{
  Serial.print("LF:");
  Serial.print(info.remote_tx.left_right);
  Serial.print("\tFB:");
  Serial.print(info.remote_tx.forw_back);
  Serial.print("\tMODE:");
  Serial.print(info.remote_tx.sta);
  Serial.print("\tSTA:");
  Serial.print(info.remote_tx.mode);
  Serial.print("\n");
}

void print_data_2()
{
  Serial.print("cur_speed:");
  Serial.print(info.car_tx.cur_speed);
  Serial.print("\tcur_dir:");
  Serial.print(info.car_tx.cur_dir);
  Serial.print("\tcur_state:");
  Serial.print(info.car_tx.cur_state);
  Serial.print("\n");
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
