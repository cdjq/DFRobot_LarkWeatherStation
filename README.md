# DFRobot_LarkWeatherStation

* [Chinese Version](./README.md)

This is a driver library for the Lark Weather Station, which allows you to retrieve data from the sensors on the Lark Weather Station.

![Product Image](./resources/images/EDG0157.png)

## Product Link (https://www.dfrobot.com.cn)

SKU: EDU0157

## Table of Contents

  * [Overview](#overview)
  * [Library Installation](#library-installation)
  * [Methods](#methods)
  * [Compatibility](#compatibility)
  * [History](#history)
  * [Authors](#authors)

## Overview

  * Retrieve data from the Lark Weather Station sensors.

## Library Installation

Before using this library, please download the library file and paste it into the \Arduino\libraries directory. Then, open the examples folder and run the demo in that folder.

## Methods

```C++
/**
 * @brief Get sensor data
 *
 * @param keys Data to be obtained
 * @return String Returns the acquired data
 */
String getValue(char *keys);

/**
 * @brief Get data unit
 *
 * @param keys Data for which units need to be obtained
 * @return String Returns the obtained units
 */
String getUnit(char *keys);

/**
 * @brief Get all data
 *
 * @param state true: include timestamp, false: do not include timestamp
 * @return String Returns all the acquired data
 */
String getInformation(bool state);

/**
 * @brief Set RTC time
 *
 * @param year Year
 * @param month Month
 * @param day Day
 * @param hour Hour
 * @param minute Minute
 * @param second Second
 * @return Returns the set status
 */
 uint8_t setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

 /**
  * @brief Get RTC time
  *
  * @return Returns the acquired RTC time
  */
  String getTimeStamp();
  /**
   * @fn setSpeed1
   * @brief Set standard wind speed 1.
   * 
   * @param speed Data for standard wind speed 1
   */
  void setSpeed1(float speed);
  /**
   * @fn setSpeed2
   * @brief Set standard wind speed 2.
   * 
   * @param speed Data for standard wind speed 2
   */
  void setSpeed2(float speed);
  /**
   * @fn calibrationSpeed
   * @brief Start calculating data.
   * @return Status data
   */
  String calibrationSpeed(void);
  /**
   * @fn configDTU
   * 
   * @brief Configure DTU enablement.
   * @param dtuswitch DTU switch
   * @param method Operation mode
   */
  uint8_t configDTU(char* dtuswitch, char* method);
  /**
   * @fn configWIFI 
   * 
   * @brief Configure WiFi information.
   * @param SSID WiFi name
   * @param PWD WiFi password
   */
  uint8_t configWIFI(char* SSID, char* PWD);
  /**
   * @fn configLora
   * 
   * @brief Configure LoRa.
   * @param DEUI Gateway
   * @param EUI Node
   * @param KEY Key
   */
  uint8_t configLora(char* DEUI, char* EUI, char* KEY);
  /**
   * @fn configMQTT1
   * 
   * @brief MQTT configuration 1.
   * @param Server MQTT platform
   * @param Server_IP MQTT platform IP
   * @param Save Whether to save transmitted data
   */
  uint8_t configMQTT1(char* Server, char* Server_IP, char* Save);
  /**
   * @fn configMQTT2
   * 
   * @brief MQTT configuration 2.
   * @param Iot_ID Login username
   * @param Iot_PWD Login password
   */
  uint8_t configMQTT2(char* Iot_ID, char* Iot_PWD);
  /**
   * @fn configTopic
   * 
   * @brief Topic subscription.
   * @param name Topic name
   * @param chan Key
   */
  uint8_t configTopic(char* name, char* chan);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |


## History

- 2023/06/8 - Version 1.0.0

## Authors

Written by TangJie (jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))