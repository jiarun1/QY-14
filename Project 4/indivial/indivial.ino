

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(3,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print((float)analogRead(A0)*5.0/1024.0);
  Serial.print('\t');
  Serial.println(digitalRead(3));
  delay(100);
}
