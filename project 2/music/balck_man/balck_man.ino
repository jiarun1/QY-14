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
int tune[] =
{
D4,D4,D4,D4,D6,D6,D6,D6,D5,D5,D5,D5,HD1,HD1,HD1,HD1,HD2,HD2,HD2,HD2,HD2,HD2,HD2,HD2,D5,D4,D3,D1,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,
D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,
HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D4,D4,D4,D4,D6,D6,D6,D6,D5,D5,D5,D5,HD1,HD1,HD1,HD1,HD2,HD2,HD2,HD2,HD2,HD2,HD2,HD2,D5,D4,D3,D1,D2,D0,D2,D6,D5,
D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,D6,D5,D0,D4,D0,D3,D0,D3,D3,D5,D0,D4,D3,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D2,D0,D2,HD4,HD3,HD4,HD3,HD4,D0,D0,D0,D0
};

int length;
int gequ = 9;
void setup()
{
  pinMode(gequ, OUTPUT);
  length = sizeof(tune) / sizeof(tune[0]);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(4,HIGH);
  delay(1500);
  digitalWrite(5,HIGH);
  delay(1500);
  digitalWrite(6,HIGH);
  
}
void loop() {

  for (int x = 0; x < length; x++)
  {
    tone(gequ, tune[x]);
    delay(400*0.5);
    noTone(gequ);
    delay(30);
  }

}
