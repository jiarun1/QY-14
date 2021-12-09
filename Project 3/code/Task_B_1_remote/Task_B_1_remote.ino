/*
	Name:       bicommunication.ino
	Created:	2021/12/4 23:35:24
	Author:     LAPTOP-MR5CQQ9V\LJR
*/

// Define User Types below here or use a .h file
//
#include <LiquidCrystal.h>
#include <SPI.h>
#include "RF24.h"
#include <MsTimer2.h>



#define   joystick_X    A0
#define   joystick_Y    A1
#define   joystick_Key  2

#define rs                  3
#define enable              4
#define d4                  5
#define d5                  6
#define d6                  7
#define d7                  8 

#define RF24_CN             9
#define RF24_CEN            10
//OUTPUT ROW AND COLOUM NUMBER
#define LCD_STA_ROW   0
#define LCD_STA_COL   4
#define LCD_SPEED_ROW 0
#define LCD_SPEED_COL 13
#define LCD_SPEED_2_ROW 1
#define LCD_SPEED_2_COL 13
#define LCD_DIS_ROW   1
#define LCD_DIS_COL   4

#define ONE_METER	1800.0


// Define Function Prototypes that use User Types below here or use a .h file
//
#define RF24_CHANNAL_TX		100

#define CONTROL_PERIOD		10

//1 : left || -1: right
const int motor_position[4] = { 1,1,-1,-1 };

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
		long distance;//unsigned long distance;
	}car_tx;
}information;

bool keep_speed = false;

void transform(void);

/********************        LCD          ************************/
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

//global variable
RF24 rf24(RF24_CN, RF24_CEN);
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


  rf24.openWritingPipe(address[0]); // 00002
  rf24.openReadingPipe(pipe, address[1]); // 00001
  
	rf24.setPALevel(RF24_PA_MAX);
	rf24.setDataRate(RF24_2MBPS);
	Serial.begin(9600);

	joystick_init();
	LCD_task_Init();
	Timer_init();
	Serial.println("init success");

}

void Timer_init(void)
{
	MsTimer2::set(CONTROL_PERIOD, HMI_control);
	MsTimer2::start();
}

void joystick_init(void)
{
	//pinMode(joystick_X, INPUT);
	//pinMode(joystick_Y, INPUT);
	pinMode(joystick_Key, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(joystick_Key), keep_speed_button,FALLING);
}

void keep_speed_button(void)// the button in joysticks
{
	keep_speed = !keep_speed;
}

void HMI_control(void)
{
  interrupts();
	encode_motor(analogRead(joystick_X),analogRead(joystick_Y));
	LCD_print_feedback();
}

inline void LCD_task_Init(void)
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("STA:    SPL:");
	lcd.setCursor(0, 1);
	lcd.print("DIS:      R:");
}

inline void LCD_print_feedback(void)
{
	lcd.setCursor(LCD_STA_COL, LCD_STA_ROW);
	if (info.car_tx.state == 0)lcd.print("Stop");
	else lcd.print("Run");
	
	lcd.setCursor(LCD_SPEED_COL, LCD_SPEED_ROW);
	lcd.print((int)(info.car_tx.speed_1));

  lcd.setCursor(LCD_SPEED_2_COL, LCD_SPEED_2_ROW);
  lcd.print((int)(info.car_tx.speed_2));
	
	lcd.setCursor(LCD_DIS_COL, LCD_DIS_ROW);
	lcd.print((long)(info.car_tx.distance)/100);
}


void encode_motor(int X, int Y)
{
	if (keep_speed == true)return;
	for (int i = 0;i < 4;i++)
	{
		info.remote_tx.motor_speed[i] = (X - 518) *0.2;    //*200 / 400;//256;
	}
	for (int i = 0;i < 4;i++)
	{
		info.remote_tx.motor_speed[i] += motor_position[i] * ((Y - 517) *0.2);//*200 /400);//256);
    if(info.remote_tx.motor_speed[i]>100)info.remote_tx.motor_speed[i]=100;
    if(info.remote_tx.motor_speed[i]<-100)info.remote_tx.motor_speed[i]=-100;
	} 
}

// Add the main program code into the continuous loop() function
void loop()
{
  /*
  info.remote_tx.motor_speed[0] = 10;
  info.remote_tx.motor_speed[1] = 20;
  info.remote_tx.motor_speed[2] = 30;
  info.remote_tx.motor_speed[3] = 70;*/
  
	transform();
}


void transform(void)
{
  send_data((uint8_t*)&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
  receive((uint8_t*)&(info.car_tx), (uint8_t)sizeof(info.car_tx), 10000);
  print_data_2();
}


bool receive(void* data, uint8_t size1, long timelimit)
{
  bool st = false;
  rf24.startListening();
  long time_limit_tmp = micros();
  while ((micros() - time_limit_tmp) <= timelimit)
  {
    if (rf24.available())
    {
      rf24.read((uint8_t*)data, size1);
      st = true;
      break;
    }
  }
  rf24.stopListening();
  return st;
}

void send_data(void* data, uint8_t size1)
{
  rf24.stopListening();
  bool send_tmp = rf24.write((uint8_t*)data, (uint8_t)size1);
  Serial.println(send_tmp);
}

void print_data()
{
  Serial.print("motor1:");
  Serial.print(info.remote_tx.motor_speed[0]);
  Serial.print("\tmotor2:");
  Serial.print(info.remote_tx.motor_speed[1]);
  Serial.print("\tmotor3:");
  Serial.print(info.remote_tx.motor_speed[2]);
  Serial.print("\tmotor4:");
  Serial.println(info.remote_tx.motor_speed[3]);
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
