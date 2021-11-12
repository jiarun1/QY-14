#include <SoftwareSerial.h>
#include <MsTimer2.h>


#define RX_Pin      4
#define TX_Pin      5

#define BUZZER_OUT  6
#define buzzerPin   6
#define gequ        6

#define TASK_A_LED_PERIOD   10
#define TASK_B_LED_PERIOD   50
#define TASK_C_LED_PERIOD   100
/***music ***/
#define D0 -1  //putong
#define D1 258
#define D2 289
#define D3 325
#define D4 345
#define D5 388
#define D6 436
#define D7 490
#define LD1 143 //xia
#define LD2 161
#define LD3 171
#define LD4 192
#define LD5 217
#define LD6 244
#define LD7 274
#define HD1 519 //shang
#define HD2 582
#define HD3 654
#define HD4 693
#define HD5 779
#define HD6 875
#define HD7 983



const int tempo=154; 
int leng;

//const.yindiao
const int c3=262,c3s=277,d3=294,d3s=311,e3=330,f3=349,f3s=370,g3=392,g3s=415,a3=440,a3s=466,b3=494;
const int c4=523,c4s=554,d4=587,d4s=622,e4=659,f4=698,f4s=740,g4=784,g4s=831,a4=880,a4s=932,b4=988;
const int c5=1047,c5s=1109,d5=1175,d5s=1245,e5=1319,f5=1397,f5s=1480,g5=1568,g5s=1661,a5=1760,a5s=1865,b5=1976;
const int c6=2093,c6s=2217,d6=2349,d6s=2489,e6=2637,f6=2794,f6s=2960,g6=3136,g6s=3322,a6=3520,a6s=3729,b6=3951;
//const.changdu
const int _1=240000/tempo,_2=120000/tempo,_4=60000/tempo,_8=30000/tempo,_16=15000/tempo,_32=7500/tempo,_64=3750/tempo;
const int _1t=160000/tempo,_2t=80000/tempo,_4t=40000/tempo,_8t=20000/tempo,_16t=10000/tempo,_32t=5000/tempo,_64t=2500/tempo;
const int _1d=360000/tempo,_2d=180000/tempo,_4d=90000/tempo,_8d=45000/tempo,_16d=22500/tempo,_32d=11250/tempo;
/*********************/
//music 3
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
const int ledPin1 = 12;
const int ledPin2 = 13;
 
int counter = 0;
bool LED_STA = 0;
/****************************/

SoftwareSerial buzzer_board(RX_Pin, TX_Pin);


int tune[] =
{
D4,D4,D4,D4,D6,D6,D6,D6,D5,D5,D5,D5,HD1,HD1,HD1,HD1,HD2,HD2,HD2,HD2,HD2,HD2,HD2,HD2,D5,D4,D3,D1,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,
D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,
HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D4,D4,D4,D4,D6,D6,D6,D6,D5,D5,D5,D5,HD1,HD1,HD1,HD1,HD2,HD2,HD2,HD2,HD2,HD2,HD2,HD2,D5,D4,D3,D1,D2,D0,D2,D6,D5,
D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D0,D0,D0,D0
};



void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT);
  leng = sizeof(tune) / sizeof(tune[0]);
  buzzer_board.begin(19200);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char c = (char)buzzer_board.read();
  while(c == -1)
  {
    c = (char)buzzer_board.read();Serial.print(c);
  }
  Serial.print('\n');
  Serial.print(c);
  
  switch(c)
  {
    case '0':task_A_music();break;
    case '1':task_B_music();break;
    case '2':task_C_music();break;
  }
}

void task_A_music(void)
{
  for (int x = 0; x < leng; x++)
  {
    tone(gequ, tune[x]);
    delay(400*0.5);
    noTone(gequ);
    delay(30);
  }
}

void task_B_music(void)
{
  //  1
tone(buzzerPin,d4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_8);
//  2
tone(buzzerPin,d4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_4);
tone(buzzerPin,f4);
delay(_4);
//  3
tone(buzzerPin,d4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_8);
//  4
tone(buzzerPin,d4);
delay(_4);
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,g4);
delay(_4);
tone(buzzerPin,a4);
delay(_4);
//  sub
buzhidao();
nihaoya();
}

void buzhidao()
{
for(int i=0;i<3;i++)
{//  5
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
}
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,e4);
delay(_16t);
tone(buzzerPin,f4);
delay(_16t);
tone(buzzerPin,e4);
delay(_16t);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
for(int i=0;i<2;i++)
{//  7
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
}
//  8
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,f5);
delay(_8);
tone(buzzerPin,e5);
delay(_16);
tone(buzzerPin,f5);
delay(_16);
tone(buzzerPin,e5);
delay(_16);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
for(int i=0;i<3;i++)
{//  9
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,c4);
delay(_16);
}
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,e4);
delay(_16t);
tone(buzzerPin,f4);
delay(_16t);
tone(buzzerPin,e4);
delay(_16t);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
//  11
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,f4);
delay(_16);
tone(buzzerPin,g4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,g4);
delay(_16);
tone(buzzerPin,a4);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
tone(buzzerPin,f5);
delay(_16);
tone(buzzerPin,a4);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
//  12
tone(buzzerPin,f5);
delay(_8);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,f5);
delay(_16t);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,d5);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,f5);
delay(_8);
for(int i=0;i<3;i++)
{//  13
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
}
tone(buzzerPin,f5);
delay(_8);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,f5);
delay(_16t);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,c5);
delay(_8);
for(int i=0;i<2;i++)
{//  15
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
}
//  16
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
tone(buzzerPin,c6);
delay(_8);
tone(buzzerPin,f6);
delay(_8);
tone(buzzerPin,e6);
delay(_16);
tone(buzzerPin,f6);
delay(_16);
tone(buzzerPin,e6);
delay(_16);
tone(buzzerPin,d6);
delay(_16);
tone(buzzerPin,c6);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
for(int i=0;i<3;i++)
{//  17
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
tone(buzzerPin,d5);
delay(_16);
tone(buzzerPin,c5);
delay(_16);
}
tone(buzzerPin,f5);
delay(_8);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,f5);
delay(_16t);
tone(buzzerPin,e5);
delay(_16t);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,c5);
delay(_8);
//  19
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_16);
tone(buzzerPin,c6);
delay(_16);
tone(buzzerPin,d6);
delay(_16);
tone(buzzerPin,c6);
delay(_16);
tone(buzzerPin,a5);
delay(_16);
tone(buzzerPin,g5);
delay(_16);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,f5);
delay(_8);
tone(buzzerPin,g5);
delay(_8);
tone(buzzerPin,a5);
delay(_8);
//  20
tone(buzzerPin,d5);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,d5);
delay(_8d);
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,d5);
delay(_2);
//  21
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
//  22
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,a3);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
//  23
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
noTone(buzzerPin);
//  24
tone(buzzerPin,a4);
delay(_4);
tone(buzzerPin,g4);
delay(_8t);
tone(buzzerPin,a4);
delay(_8t);
tone(buzzerPin,g4);
delay(_8t);
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
//  25
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
//  26
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,a3);
delay(_8);
//  27
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8d);
tone(buzzerPin,c4);
delay(_16);
noTone(buzzerPin);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
//  28
tone(buzzerPin,a4);
delay(_4);
tone(buzzerPin,g4);
delay(_8t);
tone(buzzerPin,a4);
delay(_8t);
tone(buzzerPin,g4);
delay(_8t);
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,d4);
delay(_4);
}
void nihaoya()
{//  29
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,e4);
delay(_4);
tone(buzzerPin,d4);
delay(_4);
tone(buzzerPin,c4);
delay(_4);
noTone(buzzerPin);
//  30
tone(buzzerPin,c4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,a3);
delay(_8);
tone(buzzerPin,g3);
delay(_8);
tone(buzzerPin,a3);
delay(_2);
noTone(buzzerPin);
//  31
tone(buzzerPin,a3);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_4);
tone(buzzerPin,g4);
delay(_4);
tone(buzzerPin,e4);
delay(_4);
//  32
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,e4);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
tone(buzzerPin,d4);
delay(_2);
//  33
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,e4);
delay(_4);
tone(buzzerPin,d4);
delay(_4);
tone(buzzerPin,c4);
delay(_4);
noTone(buzzerPin);
//  34
tone(buzzerPin,c4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,c4);
delay(_16);
tone(buzzerPin,d4);
delay(_16);
tone(buzzerPin,a3);
delay(_8);
tone(buzzerPin,g3);
delay(_8);
tone(buzzerPin,a3);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,a3);
delay(_8);
tone(buzzerPin,c4);
delay(_8);
//  35
tone(buzzerPin,d4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_4);
noTone(buzzerPin);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,g4);
delay(_4);
//  36
tone(buzzerPin,e4);
delay(_2d);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
//  37
tone(buzzerPin,g4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,g4);
delay(_8d);
tone(buzzerPin,a4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_4d);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_8);
//  38
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,a4);
delay(_4);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
//  39
tone(buzzerPin,g4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,g4);
delay(_8d);
tone(buzzerPin,a4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_4d);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_8);
//  40
tone(buzzerPin,a4s);
delay(_8);
tone(buzzerPin,a4);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,f4);
delay(_4);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
//  41
tone(buzzerPin,g4);
delay(_8d);
noTone(buzzerPin);
tone(buzzerPin,g4);
delay(_8d);
tone(buzzerPin,a4);
delay(_8);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_4d);
noTone(buzzerPin);
tone(buzzerPin,a4);
delay(_8);
//  42
tone(buzzerPin,c5);
delay(_8);
tone(buzzerPin,d5);
delay(_8);
tone(buzzerPin,g4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
tone(buzzerPin,a4);
delay(_4);
tone(buzzerPin,d4);
delay(_8);
tone(buzzerPin,f4);
delay(_8);
//  43
tone(buzzerPin,a4s);
delay(_4);
tone(buzzerPin,a4);
delay(_4);
tone(buzzerPin,g4);
delay(_4);
tone(buzzerPin,f4);
delay(_4);
}

void task_C_music(void){
  //Play first section
  firstSection();
 
  //Play second section
  secondSection();
 
  //Variant 1
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);
 
  //Repeat second section
  secondSection();
 
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
  
}

void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
