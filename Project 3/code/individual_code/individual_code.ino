/*
 Name:		individual_code.ino
 Created:	2021/12/6 0:47:45
 Author:	LJR
*/

#include <MsTimer2.h>

#define PWM_OUTPUT_PIN		10
#define CTLx_PIN			11
#define MODE_PIN			12

bool mode = 0;
bool ctlx = 0;
int PWM = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	MsTimer2::set(100, timer2);
	MsTimer2::start();
}

void timer2(void)
{
	Serial.println("current mode:");
	Serial.println(mode);
	Serial.println("current mode:");
	Serial.println(ctlx);
	Serial.println("current mode:");
	Serial.println(PWM);

	pinMode(PWM_OUTPUT_PIN, OUTPUT);
	pinMode(CTLx_PIN, OUTPUT);
	pinMode(MODE_PIN, OUTPUT);


}



// the loop function runs over and over again until power down or reset
void loop() {
	analogWrite(PWM_OUTPUT_PIN, PWM);
	digitalWrite(CTLx_PIN, ctlx);
	digitalWrite(MODE_PIN, mode);
}
