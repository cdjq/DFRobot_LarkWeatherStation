#include "DFRobot_Atmospherlum.h"
#define DEVICE_ADDR                  0x42
DFRobot_Atmospherlum atm(DEVICE_ADDR,&Wire);
sTime_t time;
void setup(void){
  time.year = 2022;
   time.month =12;
   time.day = 8;
    time.hour = 11;
   time.minute =0;
   time.second = 0;
   time.week = 4;
  Serial.begin(115200);
  while(atm.begin()){
    Serial.println("init error");
    delay(1000);
  }
  Serial.println("init success");
  atm.setTime(time);
}

void loop(void){
  String data;
    Serial.println("----------------------------");
    Serial.println(atm.getTimeStamp());
    Serial.print(atm.getValue("Speed"));
    Serial.println(atm.getUnit("Speed"));
     Serial.print(atm.getValue("Temp"));
    Serial.println(atm.getUnit("Temp"));
    Serial.print(atm.getValue("Humi"));
    Serial.println(atm.getUnit("Humi"));
    Serial.print(atm.getValue("Pressure"));
    Serial.println(atm.getUnit("Pressure"));
    Serial.print(atm.getValue("Altitude"));
    Serial.println(atm.getUnit("Altitude"));
    Serial.print(atm.getValue("Battery"));
    Serial.println(atm.getUnit("Battery"));
    Serial.println("----------------------------");
    data = atm.getInformation(true);
    Serial.println(data);
    delay(1000);
}