#include <SPI.h>
#include "RF24.h"

#define RF24_CHANNAL_TX    100
//#define RF24_CHANNAL_RX   101

#define MODE 2


RF24 rf24(9, 10); // CE, CSN

const byte address[][6] = {"1NODE", "2NODE" };
const byte pipe = 1;

bool transform(void);

typedef struct
{
  struct
  {
    int left_right;
    int forw_back;
    uint8_t sta;//run: 1 || stop :0
    uint8_t mode;//circles: 1|| moving turn:0;
  }remote_tx;
  struct 
  {
    int cur_speed;
    int cur_dir;
    bool cur_state;
  }car_tx;

}information;

information info,rece;
// the setup function runs once when you press reset or power the board
void setup() 
{
  //pinMode(button, INPUT);
  Serial.begin(9600);
  rf24.begin();
  rf24.setChannel(RF24_CHANNAL_TX);
  rf24.setPALevel(RF24_PA_MAX);
#if (MODE==1)
  rf24.openWritingPipe(address[0]); // 00002
#elif (MODE==2)
  rf24.openReadingPipe(pipe, address[0]);
#endif
  Serial.println("test");

}
void loop() {
//    Serial.println("ok");

#if (MODE==1)
  info.remote_tx.left_right = 10;
  info.remote_tx.forw_back = 70;
  info.remote_tx.mode = 1;
  info.remote_tx.sta = 0;
  transform();
#elif (MODE==2)
  //Serial.println("ok1");
  transform();
  {
    Serial.println("LF:");
    Serial.println(rece.remote_tx.left_right);
    Serial.println("FB:");
    Serial.println(rece.remote_tx.forw_back);
    Serial.println("MODE:");
    Serial.println(rece.remote_tx.mode);
    Serial.println("STA:");
    Serial.println(rece.remote_tx.sta);
  //delay(1000);*/
  }
#endif
}

const char msg[] = "Happy Hacking!";

bool suc = false;
bool transform(void)
{
#if (MODE==1)
  rf24.stopListening();
  //suc = rf24.write((uint8_t*)&(info.remote_tx.left_right), (uint8_t)sizeof(info.remote_tx.left_right));
  suc = rf24.write(&(info.remote_tx), (uint8_t)sizeof(info.remote_tx));
  Serial.println(suc);
  delay(100);
  return suc;

#elif (MODE==2)
  rf24.startListening();
  delay(1);
  if (rf24.available(&pipe)) {
    //rf24.read((uint8_t*)&(info.remote_tx.left_right), (uint8_t)sizeof(info.remote_tx.left_right));
    rf24.read((uint8_t*)&(rece.remote_tx), (uint8_t)sizeof(rece.remote_tx));

  }
  
  rf24.stopListening();
  return suc;

#endif
}
