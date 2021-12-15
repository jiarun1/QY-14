/*
 Name:		Line_following_1.ino
 Created:	2021/12/12 16:04:14
 Author:	LJR
*/

#include <MsTimer2.h>
#include <Wire.h>           // Include the Wire library for I2C communication
#include <LiquidCrystal.h>  // Include the Liquid Crystal library for driving the LCD

// PID Loop Constant analog inputs

#define ECD_READ_CLOCK      2
#define ECD_PIN_B           11


#define rs                  3
#define enable              4
#define d4                  5
#define d5                  6
#define d6                  7
#define d7                  8 

#define BOTTON1_Pin 9 //<< pull up
#define BOTTON2_Pin 10 //<< pull up
#define BOTTON_STOP 11

// Default speed of the motors, this is modified by the PID output
#define leftMotorBaseSpeed 20
#define rightMotorBaseSpeed 20

#define Kp_mapping  10000
#define Ki_mapping  10000
#define Kd_mapping  10000

// Resolution of the normaised outputs from the sensor board (10-bit results are constrained to the calibration limits then remapped to this range)
#define outputRange 255

#define MIDDLE_POSITION 62

#define PID_MAX_IOUT    100
#define PID_MAX_OUT     200

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
    int left_sensor;
    int right_sensor;
}sensor_struct;

typedef struct
{
    int speed_1;
    bool dir;

}motor_struct;

motor_struct motor_control[4];

PID Line_pid;
PID_Param old_pid_param;
sensor_struct sensor_data;

LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);  // Configure the LCD connection


int now_position;

int motor_speed[4];
const int motor_speed_init[4] = { 40,40,40,40 };
const int motor_position[4] = { 1,1,-1,-1 };//1 for left,-1 for right;

unsigned int sensorData[8];     // Array for formatted data from module
int sensorWCal[8] = {8,8,8,8,8,8,8,8}; 
//int sensorWCal[8] = {1023,1023,1023,1023,1023,1023,1023,1023};  // Calibration values for sensor (initalised to full range [10-bit results])
int sensorBCal[8] = {0,0,0,0,0,0,0,0};

float distances[8] = {13, 13, 39, 52, 65, 78, 104, 104};   // Distance values for each sensor (referenced to imaginary sensor 13mm from others)

int encoderPos = 0;
float encoder_out = 0;


void setup()
{
    Wire.begin();        // Join the I2C bus as a master (address optional for master)
    Serial.begin(9600);  // Start serial for output to PC
    Serial.println("ok");
    Input_init();
    //LCD_task_Init();
    PID_init(&Line_pid, 2, 0.1, 0.1);
    //updatePID();
    //lcd.setCursor(0, 0); lcd.print("                ");  // Re-write the bottom display line
    //lcd.setCursor(0, 1); lcd.print("Running...");
    Timer_init();
    Serial.println("ok1");
    pinMode(ECD_READ_CLOCK, INPUT_PULLUP);
    pinMode(ECD_PIN_B, INPUT_PULLUP);

}

void loop()
{
    /*if (digitalRead(BOTTON_STOP)==0)
    {
        attachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK), ECD_READ, CHANGE);
        MsTimer2::stop();
        updatePID();
        lcd.setCursor(0, 0); lcd.print("                ");  // Re-write the bottom display line
        lcd.setCursor(0, 1); lcd.print("Running...");
        Timer_init();
        detachInterrupt(digitalPinToInterrupt(ECD_READ_CLOCK));
    }*/
    //Serial.println("start");
    
    //TODO: 
    /*
    while(1)
    {
    debug_1();
    }
    while(1){
    Serial.println(get_position());}
    //debug_1();*/
/*
    float x = get_position();
    Serial.print(x);
    Serial.print('\t');
    float output = PID_calculate(&Line_pid, MIDDLE_POSITION, x);//get_position()); // Calculate the PID output.
    Serial.println(output);
    
    for (int i = 0;i < 4;i++)
    {
        motor_speed[i] = motor_speed_init[i] - motor_position[i] * output;
    }
    decoding_motor(motor_speed);
    
    send_data_commend();*/
    //delay(10);
}

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
void updatePID(void)
{
    stop_vehicle();// Halt both motors so it's easier to change the values

    // Store the current PID constants
    PID_Param pid_tmp;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SEL:  last PID  ");
    lcd.setCursor(1, 0);
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
            PID_store(Line_pid);
            PID_display_init();
            PID_display(old_pid_param);
            break;
        }
    }
    while (digitalRead(BOTTON2_Pin) == 1)delay(100);

    PID_display_init();
    int set_param_number = 0;
    // HACK:
    while (digitalRead(BOTTON2_Pin) == 1)
    {
        if (digitalPinHasPWM(BOTTON1_Pin) == 0)
        {
            set_param_number++;
            lcd.setCursor(4, 1);
            encoderPos = 0;
            switch (set_param_number)
            {
            case 0: lcd.print('P');break;
            case 1: lcd.print('I');break;
            case 2: lcd.print('D');break;
            default:
                break;
            }
            delay(1000);
        }
        switch (set_param_number)
        {
        case 0: pid_tmp.kp = encoder_out;break;
        case 1: pid_tmp.ki = encoder_out;break;
        case 2: pid_tmp.kd = encoder_out;break;
        default:break;
        }
        PID_display(pid_tmp);
    }
    PID_init(&Line_pid, pid_tmp.kp, pid_tmp.ki, pid_tmp.kd);
}

inline void PID_display_init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("P:   I:   D:   ");
    lcd.setCursor(0, 1); lcd.print("SEL:       NEXT");
}

inline void PID_display(PID_Param pid_param)
{
    lcd.setCursor(2, 0);
    lcd.print((float)pid_param.kp);
    lcd.setCursor(7, 0);
    lcd.print((float)pid_param.ki);
    lcd.setCursor(12, 0);
    lcd.print((float)pid_param.kd);
}

inline void Input_init(void)
{
    pinMode(BOTTON1_Pin, INPUT_PULLUP);
    pinMode(BOTTON2_Pin, INPUT_PULLUP);
    pinMode(BOTTON_STOP, INPUT_PULLUP);
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

inline void PID_store(PID store_pid)
{
    old_pid_param.kp = store_pid.pid_param.kp;
    old_pid_param.ki = store_pid.pid_param.ki;
    old_pid_param.kd = store_pid.pid_param.kd;
}

inline void LCD_task_Init(void)
{
    lcd.begin(16, 2);
    lcd.clear();

    //TODO:add the lcd init interface
    lcd.setCursor(0, 0);
    lcd.print("STA:    SPL:");
    lcd.setCursor(0, 1);
    lcd.print("DIS:      R:");
}

inline void Timer_init(void)
{
    MsTimer2::set(10, Timer2_Task);//readSensorData);
    MsTimer2::start();
}


float get_position(void)
{
    readSensorData();
    float sum = 0;
    float sum_sensor = 0;
    for (int i = 0;i<8;i++)
    {
        sum_sensor += sensorData[i];
        sum += sensorData[i] * distances[i];
    }
    if(sensorData[0] > 60)return -50;
    if(sensorData[7] > 60)return 159;

    if((sum == 0 || sum_sensor == 0))return MIDDLE_POSITION;
    
    return (float)(sum/sum_sensor);
}

void send_data_commend(void)
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
        if(motor_control[i].speed_1 > 100)
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
        sensorData[n] += sensorBCal[n];
        sensorData[n] =128 - (sensorData[n] / sensorWCal[n]);
        //sensorData[n] = 200 - (sensorData[n] * 128 / sensorWCal[n]);
        
    }
}

void Timer2_Task(void)
{
    //readSensorData();
    //float x = get_position();
    //Serial.print(x);
    //Serial.print('\t');
    float output = PID_calculate(&Line_pid, MIDDLE_POSITION, get_position());
    interrupts();
    Serial.println(output);
    
    for (int i = 0;i < 4;i++)
    {
        //if(output * motor_position[i] < 0)
        //{
           motor_speed[i] = motor_speed_init[i] - motor_position[i] * output;
        //}
        //else motor_speed[i] = motor_speed_init[i] - motor_position[i] * 2*output;

        //motor_speed[i] = motor_speed_init[i] - motor_position[i] * output;
    }
    decoding_motor(motor_speed);
    
    send_data_commend();
}
