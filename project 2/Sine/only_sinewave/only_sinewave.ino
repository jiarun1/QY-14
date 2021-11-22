#include <MsTimer2.h>

#define OUT_PIN 5
#define pi 3.14

void Init_time2(void);

float freq = 0;// the entered frequency to represent
int time_for_period = 0; // the time after a new period begin

void setup() {
  // put your setup code here, to run once:
  noInterrupts();
  Serial.begin(9600);
  Init_time2();
  pinMode(OUT_PIN, OUTPUT);
  Serial.println("number enter present frequency(\10)");
  interrupts();
}

void loop() 
{
}

/**
 * @brief   init the timer 2 to creat a time interrupt every 1 millisecond
 */
inline void Init_time2(void)
{
  MsTimer2::set(1, Timer2ISR);
  MsTimer2::start();
}

/**
 * @brief     timer2 interrupt callback function 
 */
void Timer2ISR()
{
  //determine the time flow from period begin
  if(time_for_period >= (1000/freq))
     time_for_period = 0; //every a set period empty the time
  else time_for_period ++;//every millisecond update the value

  //calculated and output the PWM duty cycle
  analogWrite(OUT_PIN,(int)sin((float)(time_for_period *2*pi*freq/1000))*128+128);
   
}

/**
 * @brief   get the enter frequency from serial port
 */
void serialEvent()
{
  int inchar = 0;
  MsTimer2::stop();
  time_for_period = 0;
  freq = 0;         //init the frequency
  delay(10);
  if(Serial.available())
  {
      //get the entered value in float
      while((inchar = Serial.read())!= -1 )
      {
        if( inchar <= '9' && inchar >= '0')
        {
            freq = (float)(freq * 10 + inchar - '0');
        }
      } 
  }
  freq = freq/10.0;
  Serial.println("frequency is:");
  Serial.println(freq,5);
  Init_time2();
}
