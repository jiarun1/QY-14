#define encoder0PinA 2
#define encoder0PinB 11
#define encoder0Btn 3
int encoder0Pos = 0;
void setup() {
  Serial.begin(9600);
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder0Btn),dobotton,FALLING);
}

int valRotary,lastValRotary;

bool btn = false;

void loop() {
  Serial.print(btn);
  Serial.print(" ");
  Serial.print(valRotary);
  if(valRotary>lastValRotary)
  {
  Serial.print("  CW");
  }
  else
  {
    Serial.print("  CCW");
  }
  lastValRotary = valRotary;
  Serial.println(" ");
  delay(250);
}
void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
  encoder0Pos--;
  }
  else
  {
  encoder0Pos++;
  }
  valRotary = encoder0Pos/2;
}

void dobotton(void)
{
  btn = true;
}
