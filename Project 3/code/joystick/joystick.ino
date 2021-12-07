/*
 Name:		joystick.ino
 Created:	2021/12/5 23:21:19
 Author:	LJR
*/

#define JOYSTICK_X_PIN		8
#define JOYSTICK_Y_PIN		9
#define JOYSTICK_BUT_PIN	2


typedef struct
{
	int x;
	int y;
	bool button;
}joy_stick;

joy_stick	remote_control;




// the setup function runs once when you press reset or power the board
void setup() {
	joystick_init();

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}

void joystick_init(void)
{
	pinMode(JOYSTICK_X_PIN, INPUT);
	pinMode(JOYSTICK_Y_PIN, INPUT);
	pinMode(JOYSTICK_BUT_PIN, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(JOYSTICK_BUT_PIN), joystick_but_task, FALLING);
}

void joystick_but_task(void)
{
	remote_control.button = !remote_control.button;
}

void joystick_get_value(joy_stick* value)
{
	value->x = analogRead(JOYSTICK_X_PIN);
	value->y = analogRead(JOYSTICK_Y_PIN);
}