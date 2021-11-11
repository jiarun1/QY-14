#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3

SoftwareSerial mySerial(rxPin,txPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  
  Serial.begin(9600);

  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.print('a');
  char c = (char)mySerial.read();
  Serial.println(c);
  delay(1000);
}
