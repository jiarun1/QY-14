 #include <MsTimer2.h>

#define OUT_PIN 5
#define pi 3.14

typedef enum
{
  none = 0,
  sinewave = 1,
  seesaw = 2,
  triwave = 3
}mode;

void Init_time2(void);

float freq = 0;
int time_for_period = 0;
int out_level = 0;
mode choose_mode = none;

void setup() {
  // put your setup code here, to run once:
  noInterrupts();
  Serial.begin(9600);
  Init_time2();
  pinMode(OUT_PIN, OUTPUT);
  Serial.println("enter wave mode:");
  Serial.println("m0:stop,m1:sinewave,m2:seesaw,m3:triwave");
  Serial.println("only number enter present frequency(\10)");
  interrupts();
}

void loop() 
{

}

inline void Init_time2(void)
{
  MsTimer2::set(1, Timer2ISR);
  MsTimer2::start();
}

inline void mode_change(mode nowmode)
{
  if (nowmode == sinewave)
  {
    out_level = sin((float)(time_for_period *2*pi*freq/1000))*128+128;
  }
  else if(nowmode == seesaw)
  {
   // out_level = time_for_period*256 * freq/1000;
  }
  else if(nowmode == triwave)
  {
   /* if(time_for_period < 1000/freq/2)
      out_level = time_for_period*256*2 * freq/1000;
    else  
      out_level = 256 - time_for_period*256*2 * freq/1000;*/
  }
  else 
    out_level = 0;
}

void Timer2ISR()
{
  if(time_for_period >= (1000/freq)) time_for_period = 0;
     else time_for_period ++;
  analogWrite(OUT_PIN,out_level);
   
}

void serialEvent()
{
  MsTimer2::stop();
  
  time_for_period = 0;
  int inchar = 0;
  int digits = 0;
  bool modechange = false;
  freq = 0;
  delay(10);
  if(Serial.available())
  {
            
      while((inchar = Serial.read())!= -1 )
      {
        if( inchar == 'm')
          modechange = !modechange;       
        if( inchar <= '9' && inchar >= '0')
          if (modechange == false)
          {
            freq = (float)(freq * 10 + inchar - '0');
          }
          else if(modechange == true)
          {
            choose_mode = (mode)(inchar - '0');
          }
      } 
  }
  if(modechange == false)
  {
    freq = freq/10.0;
    Serial.println("frequency is:");
    Serial.println(freq,5);
    Init_time2();
  }
  else
  {
    Serial.println("mode is:");
    Serial.println(choose_mode);
    MsTimer2::start();
  }
}
