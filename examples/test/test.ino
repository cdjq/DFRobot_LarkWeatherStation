#include "DFRobot_Atmospherlum.h"
#define DEVICE_ADDR                  0x42
//DFRobot_Atmospherlum_I2C atm(DEVICE_ADDR,&Wire);
DFRobot_Atmospherlum_UART atm(&Serial1);
void setup(void){
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  while(atm.begin()!= 0){
    Serial.println("init error");
    delay(1000);
  }
  Serial.println("init success");
  atm.setTime(2023,1,11,23,59,0);
}

void loop(void){
    Serial.println("----------------------------");
    Serial.print(atm.getValue("Temp"));
    Serial.println(atm.getUnit("Temp"));
    Serial.print(atm.getValue("Humi"));
    Serial.println(atm.getUnit("Humi"));
    Serial.print(atm.getValue("speed"));
    Serial.println(atm.getUnit("speed"));
    Serial.println(atm.getValue("dir"));
    Serial.print(atm.getValue("Altitude"));
    Serial.println(atm.getUnit("Altitude"));
    Serial.print(atm.getValue("Pressure"));
    Serial.println(atm.getUnit("Pressure"));
    Serial.println("----------------------------");
    Serial.println(atm.getInformation(true));
    delay(1000);
}