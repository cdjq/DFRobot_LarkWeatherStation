/*!
 * @file getData.ino
 * @brief 这是一个获取云雀数据的例程
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license      The MIT License (MIT)
 * @author       [TangJie](jie.tang@dfrobot.com)
 * @version      V1.0.0
 * @date         2023-06-8
 * @url         https://github.com/DFRobot/DFRobot_Atmospherlum
 */
#include "DFRobot_Atmospherlum.h"
#include "SoftwareSerial.h"
#define DEVICE_ADDR                  0x42
DFRobot_Atmospherlum_I2C atm(DEVICE_ADDR,&Wire);
//SoftwareSerial mySerial(2,3);//RX=2,TX=3
//DFRobot_Atmospherlum_UART atm(&mySerial);
void setup(void){
  Serial.begin(115200);
  //mySerial.begin(9600);
  //delay(1000);
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
    Serial.print(atm.getValue("Speed"));
    Serial.println(atm.getUnit("Speed"));
    Serial.println(atm.getValue("Dir"));
    Serial.print(atm.getValue("Altitude"));
    Serial.println(atm.getUnit("Altitude"));
    Serial.print(atm.getValue("Pressure"));
    Serial.println(atm.getUnit("Pressure"));
    Serial.println("----------------------------");
    Serial.println(atm.getInformation(true));
    delay(100);
}