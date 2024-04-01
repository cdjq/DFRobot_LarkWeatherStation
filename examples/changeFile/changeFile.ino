/*!
 * @file getData.ino
 * @brief This is a routine to modify the DTU file
 * ---------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |
 * ---------------------------------------------------------------------------------------------------------------
 * 
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license      The MIT License (MIT)
 * @author       [TangJie](jie.tang@dfrobot.com)
 * @version      V1.0.0
 * @date         2023-06-8
 * @url         https://github.com/DFRobot/DFRobot_LarkWeatherStation
 */
#include "DFRobot_LarkWeatherStation.h"
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif
#define DEVICE_ADDR                  0x42

#define MODESWITCH        /*UART:*/1 /*I2C: 0*/

#if MODESWITCH
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  SoftwareSerial mySerial(/*rx =*/4, /*tx =*/5);
  DFRobot_LarkWeatherStation_UART atm(&mySerial);
#else
  DFRobot_LarkWeatherStation_UART atm(&Serial1);
#endif
#else
DFRobot_LarkWeatherStation_I2C atm(DEVICE_ADDR,&Wire);
#endif

void setup(void){
  Serial.begin(115200);
  //mySerial.begin(9600);
  //delay(1000);
  while(atm.begin()!= 0){
    Serial.println("init error");
    delay(1000);
  }
  Serial.println("init success");
  while(atm.configDTU("off","wifi") != 1){
    delay(1000);
  }
  while(atm.configWIFI("SSID","PASSWORD") != 1){
    delay(1000);
  }
  while(atm.configLora("DEUI","EUI","KEY") != 1){
    delay(1000);
  }
  while(atm.configMQTT1("Server","Server_IP","0") != 1){
    delay(1000);
  }
  while(atm.configMQTT2("Iot_ID","Iot_PWD") != 1){
    delay(1000);
  }
  while(atm.configTopic("Topic_Humi","12fd35") != 1){
    delay(1000);
  }
  while(atm.configTopic("Topic_kk","12fd35") != 1){
    delay(1000);
  }
  Serial.println("config OK");
}

void loop(void){
    
    delay(100);
}