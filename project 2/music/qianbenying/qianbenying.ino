const int buzzerPin = 9;
const int tempo=154;  
int i;

//const.yindiao
const int c3=262,c3s=277,d3=294,d3s=311,e3=330,f3=349,f3s=370,g3=392,g3s=415,a3=440,a3s=466,b3=494;
const int c4=523,c4s=554,d4=587,d4s=622,e4=659,f4=698,f4s=740,g4=784,g4s=831,a4=880,a4s=932,b4=988;
const int c5=1047,c5s=1109,d5=1175,d5s=1245,e5=1319,f5=1397,f5s=1480,g5=1568,g5s=1661,a5=1760,a5s=1865,b5=1976;
const int c6=2093,c6s=2217,d6=2349,d6s=2489,e6=2637,f6=2794,f6s=2960,g6=3136,g6s=3322,a6=3520,a6s=3729,b6=3951;
//const.changdu
const int _1=240000/tempo,_2=120000/tempo,_4=60000/tempo,_8=30000/tempo,_16=15000/tempo,_32=7500/tempo,_64=3750/tempo;
const int _1t=160000/tempo,_2t=80000/tempo,_4t=40000/tempo,_8t=20000/tempo,_16t=10000/tempo,_32t=5000/tempo,_64t=2500/tempo;
const int _1d=360000/tempo,_2d=180000/tempo,_4d=90000/tempo,_8d=45000/tempo,_16d=22500/tempo,_32d=11250/tempo;
void setup() {
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
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
for(i=0;i<3;i++)
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
for(i=0;i<2;i++)
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
for(i=0;i<3;i++)
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
for(i=0;i<3;i++)
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
for(i=0;i<2;i++)
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
for(i=0;i<3;i++)
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
