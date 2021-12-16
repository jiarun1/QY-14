/*
 Name:		Line_following_1.ino
 Created:	2021/12/12 16:04:14
 Author:	LJR
*/

#include <MsTimer2.h>
#include <Wire.h>           // Include the Wire library for I2C communication
#include <LiquidCrystal.h>  // Include the Liquid Crystal library for driving the LCD

// Pin distribution for Arduino Nano
#define ECD_READ_CLOCK      2
#define ECD_PIN_B           11

#define rs                  4
#define enable              5
#define d4                  6
#define d5                  7
#define d6                  8
#define d7                  9 

#define BOTTON1_Pin         10
#define BOTTON2_Pin         A0 
#define BOTTON_STOP         12

#define Kp_mapping          100
#define Ki_mapping          100
#define Kd_mapping          100

// Resolution of the normaised outputs from the sensor board (10-bit results are constrained to the calibration limits then remapped to this range)
#define outputRange 255

#define MIDDLE_POSITION 62

#define PID_MAX_IOUT    100
#define PID_MAX_OUT     200

#define TIMER_PERIOD    10  // in millisecond

#define LimitMax(input, max_in)   \
    {                          \
        if (input > max_in)       \
        {                      \
            input = max_in;       \
        }                      \
        else if (input < -max_in) \
        {                      \
            input = -max_in;      \
        }                      \
    }

typedef struct
{
    float kp;
    float ki;
    float kd;
}PID_Param;

typedef struct
{
    PID_Param pid_param;

    float max_out;
    float max_iout;

    float error;
    float last_error;

    float Pout;
    float Iout;
    float Dout;
    float Out;
}PID;

typedef struct
{
    int right_speed;
    int left_speed;
    int error;
    bool sta;
}LCD_RUNNING_OUT_STRUCT;

typedef struct
{
    int speed_1;
    bool dir;
}motor_struct;

motor_struct motor_control[4];

PID Line_pid;
PID_Param old_pid_param;

LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);  // Configure the LCD connection

LCD_RUNNING_OUT_STRUCT LCD_PRINT;

int now_position;

int motor_speed[4];
const int motor_speed_init[4] = { 40,40,40,40 };
const int motor_position[4] = { 1,1,-1,-1 };//1 for left,-1 for right;

unsigned int sensorData[8];     // Array for formatted data from module
//int sensorWCal[8] = {8,8,8,8,8,8,8,8}; // to mapping the data to 0-125 << 1023/125
float sensorWCal[8] = { 5.78,4.89,5.52,4.56, 4.88, 4.57,5.27, 5.75 }; // to mapping the data to 0-125 << 1023/125
// int sensorWCal[8] = {1023,1023,1023,1023,1023,1023,1023,1023};  // Calibration values for sensor (initalised to full range [10-bit results])
int sensorBias[8] = { -264,-378,-299,-420,-378,-420,-331,-270 };

float distances[8] = { 13, 13, 39, 52, 65, 78, 104, 104 };   // Distance values for each sensor (referenced to imaginary sensor 13mm from others)

unsigned long last_encoder1Value = 0x80000000;
unsigned long encoder1Value = 0x80000000;
unsigned long last_encoder2Value = 0x80000000;
unsigned long encoder2Value = 0x80000000;

int encoderPos = 0;
int encoderPos_Kp = 0;
int encoderPos_Ki = 0;
int encoderPos_Kd = 0;
float encoder_out = 0;


void setup()
{
    Wire.begin();        // Join the I2C bus as a master (address optional for master)
    Serial.begin(9600);  // Start serial for output to PC
    Serial.println("start");
    pinMode(BOTTON1_Pin, INPUT_PULLUP);
    pinMode(BOTTON2_Pin, INPUT_PULLUP);
    pinMode(BOTTON_STOP, INPUT_PULLUP);
    pinMode(ECD_READ_CLOCK, INPUT_PULLUP);
    pinMode(ECD_PIN_B, INPUT_PULLUP);
    //PID_init(&Line_pid, 2, 0.1, 0.1);
    //attachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK), ECD_READ, CHANGE);
    lcd.begin(16, 2);
    lcd.clear();
}

void loop()
{
    if (digitalRead(BOTTON_STOP) == 0)
    {
        Timer_stop();
        attachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK), ECD_READ, CHANGE);
        interrupts();
        updatePID();
        detachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK));
        while (digitalRead(BOTTON_STOP) == 0)
        {
            lcd.setCursor(0, 1);lcd.print("waiting ...");
            delay(10);
        }
        //lcd.setCursor(0, 0); lcd.print("                ");  // Re-write the bottom display line
        lcd.setCursor(0, 1); lcd.print("Running...");
        Timer_init();
    }
}

void updatePID(void)
{
    stop_vehicle();// Stop all the motor

    // Store the current PID constants
    PID_Param pid_tmp;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SEL:  last PID  ");
    lcd.setCursor(0, 1);
    lcd.print("NEXT: store PID ");
    while (1)
    {
        if (digitalRead(BOTTON1_Pin) == 0)
        {
            PID_display_init();
            PID_display(old_pid_param);
            break;
        }
        if (digitalRead(BOTTON2_Pin) == 0)
        {
            PID_store(Line_pid.pid_param);
            PID_display_init();
            PID_display(old_pid_param);
            break;
        }
    }
    while ((digitalRead(BOTTON2_Pin) == 0) || (digitalRead(BOTTON1_Pin) == 0))delay(10);//<< ignore the remain control by the switch
    while (digitalRead(BOTTON2_Pin) == 1)delay(10); //<< wait until the botton are pressed to start to input new data
    while (digitalRead(BOTTON2_Pin) == 0)delay(10);
    PID_display_init();
    int set_param_number = 0;
    bool first_in = true;
    while (digitalRead(BOTTON2_Pin) == 1)
    {
        if (digitalRead(BOTTON1_Pin) == 0 || first_in == true)
        {
            first_in = false;
            while (digitalRead(BOTTON1_Pin) == 0)delay(10);
            set_param_number++;
            if (set_param_number >= 3)set_param_number = 0;
            lcd.setCursor(4, 1);
            switch (set_param_number)
            {
            case 0: encoderPos_Kd = encoderPos; encoderPos = encoderPos_Kp; lcd.print('P'); break;
            case 1: encoderPos_Kp = encoderPos; encoderPos = encoderPos_Ki; lcd.print('I'); break;
            case 2: encoderPos_Ki = encoderPos; encoderPos = encoderPos_Kd; lcd.print('D'); break;
            default:break;
            }
        }
        Serial.println(encoder_out);
        if (encoder_out < 0) {
            encoderPos = 0;
            encoder_out = 0;
        }
        switch (set_param_number)
        {
        case 0: pid_tmp.kp = encoder_out / Kp_mapping; break;
        case 1: pid_tmp.ki = encoder_out / Ki_mapping; break;
        case 2: pid_tmp.kd = encoder_out / Kd_mapping; break;
        default:break;
        }
        PID_display(pid_tmp);
    }
    PID_init(&Line_pid, pid_tmp.kp, pid_tmp.ki, pid_tmp.kd);
}

inline void PID_display_init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("P    I    D    ");
    lcd.setCursor(0, 1); lcd.print("SEL:       NEXT");
}

inline void PID_display(PID_Param pid_param)
{
    lcd.setCursor(1, 0);
    lcd.print((float)pid_param.kp);
    lcd.setCursor(6, 0);
    lcd.print((float)pid_param.ki);
    lcd.setCursor(11, 0);
    lcd.print((float)pid_param.kd);
}

inline void PID_store(PID_Param store_pid)
{
    old_pid_param.kp = store_pid.kp;
    old_pid_param.ki = store_pid.ki;
    old_pid_param.kd = store_pid.kd;
}

inline void Timer_init(void)
{
    MsTimer2::set(TIMER_PERIOD, Timer2_Task);
    MsTimer2::start();
}

inline void Timer_stop(void)
{
    MsTimer2::stop();
}

float get_position(void)
{
    readSensorData();
    float sum = 0;
    float sum_sensor = 0;
    for (int i = 0;i < 8;i++)
    {
        sum_sensor += sensorData[i];
        sum += sensorData[i] * distances[i];
    }
    if (sensorData[0] > 60)return -50;
    if (sensorData[7] > 60)return 159;

    if ((sum == 0 || sum_sensor == 0))return MIDDLE_POSITION;

    return (float)(sum / sum_sensor);
}

void ECD_READ(void)
{
    if (digitalRead(ECD_READ_CLOCK) == digitalRead(ECD_PIN_B))
    {
        encoderPos--;
    }
    else
    {
        encoderPos++;
    }
    encoder_out = encoderPos / 2;
}

void readSensorData(void)
{
    unsigned char n;            // Variable for counter value
    unsigned char dataRaw[16];  // Array for raw data from module

    interrupts();
    // Request data from the module and store into an array
    n = 0;  // Reset loop variable
    Wire.requestFrom(9, 16);  // Request 16 bytes from slave device #9 (IR Sensor)
    while (Wire.available())  // Loop until all the data has been read
    {
        if (n < 16)
        {
            dataRaw[n] = Wire.read();  // Read a byte and store in raw data array
            n++;
        }
        else
        {
            Wire.read();  // Discard any bytes over the 16 we need
            //n = 0;
        }
    }
    noInterrupts();

    // Loop through and covert two 8 bit values to one 16 bit value
    // Raw data formatted as "MSBs 10 9 8 7 6 5 4 3", "x x x x x x 2 1 LSBs"
    for (n = 0;n < 8;n++)
    {
        sensorData[n] = dataRaw[n * 2] << 2;   // Shift the 8 MSBs up two places and store in array
        sensorData[n] += dataRaw[(n * 2) + 1];  // Add the remaining bottom 2 LSBs

        // Apply the calibration values here!
        // HACK: when data is black is higher// write is lower

        //TODO: calibrate the sensor
        sensorData[n] += sensorBias[n];
        sensorData[n] = 128 - (sensorData[n] / sensorWCal[n]);

    }
}

float PID_calculate(PID* pid, float set_value, float now)
{
    pid->last_error = pid->error;
    pid->error = set_value - now;

    pid->Pout = pid->pid_param.kp * pid->error;
    pid->Iout += pid->pid_param.ki * pid->error;
    pid->Dout = pid->pid_param.kd * (pid->error - pid->last_error);

    LimitMax(pid->Iout, pid->max_iout);
    pid->Out = pid->Pout + pid->Iout + pid->Dout;
    LimitMax(pid->Out, pid->max_out);

    return pid->Out;
}

void PID_init(PID* pid, float kp, float ki, float kd)
{
    pid->pid_param.kp = kp;
    pid->pid_param.ki = ki;
    pid->pid_param.kd = kd;

    pid->max_iout = PID_MAX_IOUT;
    pid->max_out = PID_MAX_OUT;

    pid->last_error = pid->error = pid->Iout = pid->Dout = pid->Pout = pid->Out = 0;
}

void Timer2_Task(void)
{
    float output = PID_calculate(&Line_pid, MIDDLE_POSITION, get_position());

    for (int i = 0;i < 4;i++)
    {
        motor_speed[i] = motor_speed_init[i] - motor_position[i] * output;
    }
    decoding_motor(motor_speed);
    interrupts();
    Vehicle_control_set();
    get_feedback_data();
    LCD_running_print();
}

void Vehicle_control_set(void)
{
    Wire.beginTransmission(42);
    Wire.write("ba");
    for (int i = 0;i < 4;i++)
    {
        if (motor_control[i].dir == 0)
            Wire.write("r");
        else if (motor_control[i].dir == 1)
            Wire.write("f");
    }
    for (int i = 0;i < 4;i++)
    {
        Wire.write((byte)motor_control[i].speed_1);
        Wire.write(0);
    }
    Wire.endTransmission();
}

void decoding_motor(int motor_speed[4])
{
    for (int i = 0;i < 4;i++)
    {
        if (motor_speed[i] < 0)
        {
            motor_control[i].dir = 0;
        }
        else if (motor_speed[i] > 0)
        {
            motor_control[i].dir = 1;
        }
        motor_control[i].speed_1 = abs(motor_speed[i]);
        if (motor_control[i].speed_1 > 100)
        {
            motor_control[i].speed_1 = 100;
        }
    }
}

inline void stop_vehicle(void)
{
    Wire.beginTransmission(42);
    Wire.write("ha");
    Wire.endTransmission();
}

/**
* @brief    : used to debug
* @note     : print each sensor data and calculated data
*/
inline void debug_1(void)
{
    Serial.print(sensorData[0]);
    Serial.print("\t");
    Serial.print(sensorData[1]);
    Serial.print("\t");
    Serial.print(sensorData[2]);
    Serial.print("\t");
    Serial.print(sensorData[3]);
    Serial.print("\t");
    Serial.print(sensorData[4]);
    Serial.print("\t");
    Serial.print(sensorData[5]);
    Serial.print("\t");
    Serial.print(sensorData[6]);
    Serial.print("\t");
    Serial.print(sensorData[7]);
    Serial.print("\t");
    Serial.println(get_position());
}

void get_feedback_data(void)
{
    readEncoder1();
    readEncoder2();
    unsigned long tmp;
    unsigned long tmp_2;
    bool speed_dir_1;
    bool speed_dir_2;


    if (last_encoder1Value >= encoder1Value) { tmp = (last_encoder1Value - encoder1Value);speed_dir_1 = -1; }
    else if (last_encoder1Value < encoder1Value) { tmp = encoder1Value - last_encoder1Value;speed_dir_1 = 1; }
    if (tmp > 50)tmp = 0;

    if (last_encoder2Value >= encoder2Value) { tmp_2 = (last_encoder2Value - encoder2Value);speed_dir_2 = -1; }
    else if (last_encoder2Value < encoder2Value) { tmp_2 = encoder2Value - last_encoder2Value;speed_dir_2 = 1; }
    if (tmp > 50)tmp_2 = 0;

    Serial.println(tmp);
    LCD_PRINT.sta = ((tmp < 10) ? 0 : 1);
    LCD_PRINT.left_speed = (int)(speed_dir_1 * (int)(tmp * 0.55));//VEHICLE_CONTROL_speed_1_CAL / ONE_METER);
    LCD_PRINT.right_speed = (int)(speed_dir_2 * (int)(tmp * 0.55));
    LCD_PRINT.error = (int)Line_pid.error;
    //print_data_2();
}

void LCD_running_print(void)
{
    lcd.setCursor(0, 1);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    if (LCD_PRINT.sta == 0) lcd.print('S');
    else lcd.print('R');
    lcd.setCursor(2, 1);
    lcd.print((int)LCD_PRINT.left_speed);
    lcd.setCursor(5, 1);
    lcd.print((int)LCD_PRINT.right_speed);
    lcd.setCursor(8, 1);
    lcd.print(LCD_PRINT.error);
}

void readEncoder1(void)
{
    long unsigned int encoder1 = 0;
    Wire.beginTransmission(42);
    Wire.write("i1");
    Wire.endTransmission();
    Wire.requestFrom(42, 8);
    {
        encoder1 = (long unsigned int) Wire.read();
        encoder1 += ((long unsigned int) Wire.read() << 8);
        encoder1 += ((long unsigned int) Wire.read() << 16);
        encoder1 += ((long unsigned int) Wire.read() << 24);
    }
    last_encoder1Value = encoder1Value;
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

void readEncoder2(void)
{
    long unsigned int encoder2 = 0;
    Wire.beginTransmission(42);
    Wire.write("i2");
    Wire.endTransmission();
    //delay(1);
    Wire.requestFrom(42, 8);
    //delay(4);
    //if(Wire.available()==8)
    {
        encoder2 = (long unsigned int) Wire.read();
        encoder2 += ((long unsigned int) Wire.read() << 8);
        encoder2 += ((long unsigned int) Wire.read() << 16);
        encoder2 += ((long unsigned int) Wire.read() << 24);
    }
    last_encoder2Value = encoder2Value;
    encoder2Value = encoder2;
    if (encoder2Value > 0x80040000)
    {
        encoder2Value = 0x80000000;
    }
    else if (encoder2Value < 0x7FFC0000)
    {
        encoder2Value = 0x80000000;
    }
}
