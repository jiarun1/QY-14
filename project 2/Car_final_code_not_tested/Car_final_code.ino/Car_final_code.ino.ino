#include <Wire.h>
#include <MsTimer2.h>
#include "LiquidCrystal.h"

/*------------pin for each components start--------*/
//ECD related pin
#define ECD_READ_CLOCK      2
#define NEXT_TASK_SELECT    3
//liquidCrystal
#define rs                  4
#define enable              5
#define d4                  6
#define d5                  7
#define d6                  8
#define d7                  9

#define BUZZER_OUT          10
#define encoder0PinB        11
#define Direction_BUTTON    12

#define TASK_SEL_BOTTON     A1
#define LED_PIN_OUT         A2
#define MICRO_READ_PIN      A7

/*------------pin for each components end--------*/

//OUTPUT ROW AND COLOUM NUMBER
#define LCD_TASK_ROW  0
#define LCD_TASK_COL  5
#define LCD_DIR_ROW   0
#define LCD_DIR_COL   9
#define LCD_SPEED_ROW 0
#define LCD_SPEED_COL 14
#define LCD_TAR_ROW   1
#define LCD_TAR_COL   4
#define LCD_STA_ROW   1
#define LCD_STA_COL   12

//Pussive buzzer Frequency
#define TASKA_FREQ  30
#define TASKB_FREQ  247
#define TASKC_FREQ  1000

//set the MAXIMUM input data from the encoder
#define TASK_BC_MAX_DISTANCE    4.00
#define TASK_BC_MIN_DISTANCE    2.00
#define TASK_BC_DISTANCE_DELTA  0.1

//define the LED bright period
#define TASK_A_LED_PERIOD   100
#define TASK_B_LED_PERIOD   500
#define TASK_C_LED_PERIOD   1000

//define the max speed and min speed
#define MAX_SPEED   80
#define MIN_SPEED   50

// wait a few second to start the task
#define TASK_START_TIME     3000

// to limit the maximum and minimum value
#define MAX_MIN_LIMIT(NOW,MIN,MAX)  \
        {                           \
          if((*NOW)>(MAX))(*NOW)=(MAX);       \
          if((*NOW)<(MIN))(*NOW)=(MIN);       \
        }  

/*------------car encoder related start----------------*/
#define ONE_METER (1800)
/*------------car encoder related end----------------*/

bool can_change_task = false;

// use a struct to store the task of the vehicle
typedef struct
{
    int num;
    bool dir;
    int speed_set;
    float target;
    bool sta;
}task_type;

// use a struct to store the LCD information
typedef struct
{
    char taskname;
    char directionname[2];
    char targetname[3];
}lcd_unit;

int encoderPos = 0;
float encoder_out = 0;
int time_from_start = 0;
task_type car_task;

//create a LCD struct
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

// LCD output unit
const lcd_unit LCD_out_unit[3] = { {'A',{'C','A'},"R"},{'B',{'C','A'},"m"},{'C',{'F','B'},"m"} };

/*------------------------FOR TASK VARIABL----------------------**/
//ecd value for the vehicle
long unsigned int encoder1Value = 0;
long unsigned int encoder2Value = 0;
long unsigned int encoder3Value = 0;
long unsigned int encoder4Value = 0;

//set the turning value for 90 degrees
int turn_value = 543;
// set the offset motor speed
int motor_offset[4] = { 0,0,-4,-5 };
/******************************************************************/

bool LED_STA = 0;
int period_for_LED;


void setup() {

    //pin init for the encoder
    pinMode(ECD_READ_CLOCK, INPUT_PULLUP);
    pinMode(NEXT_TASK_SELECT, INPUT_PULLUP);
    pinMode(encoder0PinB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK), ECD_READ, CHANGE);
    attachInterrupt(digitalPinToInterrupt(NEXT_TASK_SELECT), task_change, FALLING);

    //pin init for the LCD
    pinMode(rs, OUTPUT);
    pinMode(enable, OUTPUT);
    pinMode(d4, OUTPUT);
    pinMode(d5, OUTPUT);
    pinMode(d6, OUTPUT);
    pinMode(d7, OUTPUT);

    //pin init for the HMI design(LED, BOTTOM, MICROPHONE, BUZZER);
    pinMode(LED_PIN_OUT, OUTPUT);
    pinMode(MICRO_READ_PIN, INPUT);
    pinMode(TASK_SEL_BOTTON, INPUT_PULLUP);
    pinMode(Direction_BUTTON, INPUT_PULLUP);
    pinMode(BUZZER_OUT, OUTPUT);

    //LCD init
    lcd.begin(16, 2);
}

void loop() {
    //LCD Init Interface
    lcd.setCursor(0, 0);
    lcd.print("Wait Task select");

    // wait for the sound sener get the value
    while (analogRead(MICRO_READ_PIN) < 400)continue;
    
    //select the task
    task_select();

    //choose the task
    if (car_task.num == 0)
    {
        task_A_mission();
    }
    else if (car_task.num == 1)
    {
        task_B_mission();
    }
    else if (car_task.num == 2)
    {
        task_C_mission();
    }
}

/**
* @brief    display the LCD Init Interface
* @usage    task_select
*/
inline void LCD_task_Init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TASK:  D:  SP:  ");
    lcd.setCursor(0, 1);
    lcd.print("TAR:      S:    ");
}

/**
* @brief    display the task number
* @usage    task_select
*/
inline void LCD_task_num(void)
{
    lcd.setCursor(LCD_TASK_COL, LCD_TASK_ROW);
    lcd.print(LCD_out_unit[car_task.num].taskname);
}

/**
* @brief    display the direction
* @usage    task_select
*/
inline void LCD_task_direction(void)
{
    lcd.setCursor(LCD_DIR_COL, LCD_DIR_ROW);
    lcd.print((char)LCD_out_unit[car_task.num].directionname[car_task.dir]);
}

/**
* @brief    display the speed
* @usage    task_select
*/
inline void LCD_task_speed(void)
{
    lcd.setCursor(LCD_SPEED_COL, LCD_SPEED_ROW);
    lcd.print((int)(car_task.speed_set));
}

/**
* @brief    display the target
* @usage    task_select
*/
inline void LCD_task_target(void)
{
    lcd.setCursor(LCD_TAR_COL, LCD_TAR_ROW);
    lcd.print((float)car_task.target);
    lcd.print(LCD_out_unit[car_task.num].targetname);
}

/**
* @brief    display the status
* @usage    task_select
*/
inline void LCD_task_status(void)
{
    lcd.setCursor(LCD_STA_COL, LCD_STA_ROW);
    if (car_task.sta == 0)
    {
        lcd.print("stop");
    }
    else if (car_task.sta == 1)
    {
        lcd.print("run");
    }
}

/**
* @brief    select the task
* @usage    loop
*/
void task_select(void)
{
    //LCD Init
    LCD_task_Init();

    can_change_task = false;
    // wait the task change botton is pushed
    //choose the task
    while (can_change_task == false)
    {
        if (digitalRead(TASK_SEL_BOTTON) == 0)car_task.num++;
        if (car_task.num >= 3)car_task.num = 0;
        LCD_task_num();
        delay(100);
    }
    delay(500);
    can_change_task = false;

    // wait the task change botton is pushed
    //choose the direction
    while (can_change_task == false)
    {
        car_task.dir = digitalRead(Direction_BUTTON);
        LCD_task_direction();
    }
    delay(500);
    encoderPos = 0;
    can_change_task = false;

    //choose the speed
    while (can_change_task == false)
    {
        car_task.speed_set = encoder_out + 50;
        MAX_MIN_LIMIT(&car_task.speed_set, MIN_SPEED, MAX_SPEED);
        delay(200);
        LCD_task_speed();
    }
    delay(500);
    can_change_task = false;
    encoderPos = 0;

    //choose the target
    while (can_change_task == false)
    {
        car_task.target = (float)encoder_out / 10.0;
        delay(200);
        LCD_task_target();
    }
    delay(500);
    can_change_task = false;
    car_task.sta = 1;
    LCD_task_status();

}

/**
* @brief    read the encoder value(EC11)
*/
void ECD_READ()
{
    if (digitalRead(ECD_READ_CLOCK) == digitalRead(encoder0PinB))
    {
        encoderPos--;
    }
    else
    {
        encoderPos++;
    }
    encoder_out = encoderPos / 2;
}

/**
* @brief    determine if the task select botton is pushed
*/
void task_change(void)
{
    can_change_task = true;
    Serial.print("out");
}


/**
* @brief    init the timer2 interrupt
*/
inline void Init_time2(void)
{
    MsTimer2::set(1, Timer2ISR);
    MsTimer2::start();
}

/**
* @brief    timer2 interrupt callback function
*/
void Timer2ISR(void)
{
    LED_Task();
    BUZZER_Task();
    readEncoder();
    time_from_start++;
}

/**
* @brief    LED flashing task
*/
void LED_Task(void)
{
    if (car_task.num == 0)
    {
        period_for_LED = TASK_A_LED_PERIOD;
    }
    else if (car_task.num == 1)
    {
        period_for_LED = TASK_B_LED_PERIOD;
    }
    else if (car_task.num == 2)
    {
        period_for_LED = TASK_C_LED_PERIOD;
    }

    if (time_from_start % period_for_LED == 0)
    {
        LED_STA = !LED_STA;
        digitalWrite(LED_PIN_OUT, LED_STA);
    }
}


/**
* @brief    Buzzer sound task
*/
void BUZZER_Task(void)
{
    if (car_task.num == 0)
    {
        if (time_from_start % (1 / TASKA_FREQ));
        analogWrite(BUZZER_OUT, 200);
    }
    else if (car_task.num == 1)
    {
        if (time_from_start % (1 / TASKB_FREQ));
        analogWrite(BUZZER_OUT, 200);
    }
    else if (car_task.num == 2)
    {
        if (time_from_start % (1 / TASKC_FREQ));
        analogWrite(BUZZER_OUT, 200);
    }
}

/**
* @brief    get all the encoder value
*/
void readEncoder(void)
{
    long unsigned int encoder1 = 0;
    long unsigned int encoder2 = 0;
    long unsigned int encoder3 = 0;
    long unsigned int encoder4 = 0;
    Wire.beginTransmission(42);
    Wire.write("i");
    Wire.endTransmission();
    Wire.requestFrom(42, 8);
    {
        encoder1 = (long unsigned int) Wire.read();
        encoder1 += ((long unsigned int) Wire.read() << 8);
        encoder1 += ((long unsigned int) Wire.read() << 16);
        encoder1 += ((long unsigned int) Wire.read() << 24);
        encoder2 = (long unsigned int) Wire.read();
        encoder2 += ((long unsigned int) Wire.read() << 8);
        encoder2 += ((long unsigned int) Wire.read() << 16);
        encoder2 += ((long unsigned int) Wire.read() << 24);
    }
    encoder1Value = encoder1;
    encoder2Value = encoder2;
    Wire.requestFrom(42, 8);
    {
        encoder3 = (long unsigned int) Wire.read();
        encoder3 += ((long unsigned int) Wire.read() << 8);
        encoder3 += ((long unsigned int) Wire.read() << 16);
        encoder3 += ((long unsigned int) Wire.read() << 24);
        encoder4 = (long unsigned int) Wire.read();
        encoder4 += ((long unsigned int) Wire.read() << 8);
        encoder4 += ((long unsigned int) Wire.read() << 16);
        encoder4 += ((long unsigned int) Wire.read() << 24);
    }
    encoder3Value = encoder3;
    encoder4Value = encoder4;
}

/**
* @brief    only read the encoder value of motor 1
* @usage    none
*/
void readEncoder1(void)
{
    long unsigned int encoder1 = 0;
    Wire.beginTransmission(42);
    Wire.write("i1");
    Wire.endTransmission();
    delay(1);
    Wire.requestFrom(42, 8);
    delay(4);
    //if(Wire.available()==8)
    {
        encoder1 = (long unsigned int) Wire.read();
        encoder1 += ((long unsigned int) Wire.read() << 8);
        encoder1 += ((long unsigned int) Wire.read() << 16);
        encoder1 += ((long unsigned int) Wire.read() << 24);
    }
    encoder1Value = encoder1;
    if (encoder1Value > 0x80040000)
    {
        encoder1Value = 0x80000000;
    }
    else if (encoder1Value < 0x7FFC0000)
    {
        encoder1Value = 0x80000000;
    }
}

/**
* @brief    the task A code
*/
void task_A_mission(void)
{
    Serial.print("taskA_begin");

    delay(TASK_START_TIME);

    Init_time2();

    turn_angle(car_task.dir, (turn_value * 4 * car_task.target), car_task.speed_set);

    MsTimer2::stop();
}

/**
* @brief    the task B code
*/
void task_B_mission(void)
{
    delay(TASK_START_TIME);

    Init_time2();

    float distance = car_task.target;

    while (distance >= 1)
    {
        go_straight(0, ONE_METER, car_task.speed_set);
        turn_angle(car_task.dir, turn_value, car_task.speed_set);
        distance = distance - 1;
    }
    go_straight(0, (distance * ONE_METER), car_task.speed_set);
    MsTimer2::stop();
}

/**
* @brief    the taskC code
*/
void task_C_mission(void)
{
    delay(TASK_START_TIME);

    Init_time2();

    go_straight(car_task.dir, (car_task.target * ONE_METER), car_task.speed_set);

    MsTimer2::stop();
}

/**
* @brief    turn the vehicle using setting angle,speed, direction
* @param    direction   :   control the direction of the turn
* @param    angle       :   control the rotate angle of the vehicle (in encoder value)
* @param    speed       :   control the speed of the vehicle
* @return   none
* @usage    all the tasks
*/
void turn_angle(bool direction, float angle, int speed)//only for task B
{
    long unsigned int target_turn = angle;
    long unsigned int past_encoder1Value = 0;
    Wire.beginTransmission(42);
    if (direction == 0)
    {
        Wire.write("tr");
    }
    else if (direction == 1)
    {
        Wire.write("tl");
    }

    for (int i = 0;i <= 3;i++)
    {
        Wire.write(speed);
        Wire.write(0);
    }
    for (int i = 0;i <= 3;i++)
    {
        Wire.write(target_turn % 256);
        turn_value /= 256;
        Wire.write(target_turn % 256);
        turn_value /= 256;
        Wire.write(target_turn % 256);
        turn_value /= 256;
        Wire.write(target_turn % 256);
        turn_value /= 256;
    }
    Wire.endTransmission();

    // if the speed of the car is very low, stop the task
    while (((encoder1Value - past_encoder1Value) < 5) || ((encoder1Value - past_encoder1Value) > -5))
    {
        past_encoder1Value = encoder1Value;
    }
}

/**
* @brief    let the vehicle go straight using setting angle,speed, direction
* @param    direction   :   control the direction
* @param    distance    :   control the running distance of the vehicle (in encoder value)
* @param    speed       :   control the speed of the vehicle
* @return   none
* @usage    all the tasks
*/
void go_straight(bool direction, float distance, int speed)
{
    long unsigned int past_encoder1Value = 0;
    if (direction == 0)
    {
        Wire.beginTransmission(42);
        Wire.write("f");
        for (int i = 0;i <= 3;i++)
        {
            Wire.write(speed + motor_offset[i]);
            Wire.write(0);
        }
        for (int i = 0;i <= 3;i++)
        {
            Wire.write((int)distance);
            Wire.write(0);
        }
        Wire.endTransmission();
    }
    else if (direction == 1)
    {
        Wire.beginTransmission(42);
        Wire.write("r");
        for (int i = 0;i <= 3;i++)
        {
            Wire.write(car_task.speed_set + motor_offset[i]);
            Wire.write(0);
        }
        for (int i = 0;i <= 3;i++)
        {
            Wire.write((int)distance);
            Wire.write(0);
        }
        Wire.endTransmission();
    }
    // if the speed of the car is very low, stop the task
    while (((encoder1Value - past_encoder1Value) < 5) || ((encoder1Value - past_encoder1Value) > -5))
    {
        past_encoder1Value = encoder1Value;
    }
}
