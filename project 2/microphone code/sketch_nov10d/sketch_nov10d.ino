#define INPUT_PIN A2
void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT_PIN,INPUT);
  Serial.begin(9600);
}

int value = 0;
void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(INPUT_PIN);
  Serial.print(value);
  Serial.print('\n');
  delay(100);
}
