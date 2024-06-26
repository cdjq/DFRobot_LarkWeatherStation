# DFRobot_LarkWeatherStation
* [English Version](./README.md)

这是一个云雀的驱动库，可以用它来获取云雀传感器中的数据。


![产品效果图片](./resources/images/EDG0157.png)


## 产品链接（https://www.dfrobot.com.cn）

    SKU：EDU0157

## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

  * 获取云雀的采集的数据

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```C++
  /**
   * @fn begin
   * @brief 初始化SCI采集模块，主要用于初始化通信接口
   * 
   * @return int 初始化状态
   * @n       0  初始化成功
   * @n      -1  通信接口类和对象没有传入
   * @n      -2  检查硬件连接是否正确
   */
  int begin(uint32_t freq = 100000);
  /**
   * @brief 获取传感器数据
   * 
   * @param keys 需要获取的数据
   * @return String 返回获取的数据
   */
  String getValue(char *keys);
  /**
   * @brief 获取数据单位
   * 
   * @param keys 需要获取的数据
   * @return String 返回获取后的单位
   */
  String getUnit(char *keys);
  /**
   * @brief 获取全部数据
   * 
   * @param state true:加入时间戳 false:不加时间戳
   * @return String 返回获取的全部数据
   */
  String getInformation(bool state);
  /**
   * @brief 设置RTC时间
   * 
   * @param year 年
   * @param month 月
   * @param day 日
   * @param hour 时
   * @param minute 分
   * @param second 秒
   * @return 返回设置状态
   */
  uint8_t setTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
  /**
   * @brief 获取RTC时间
   * 
   * @return 返回获取的RTC时间
   */
  String getTimeStamp(void);
  /**
   * @fn setRadius
   * @brief 设置风杯半径
   * 
   * @param radius 风杯半径
   * @return int 返回设置状态
   */
  int setRadius(float radius);
  /**
   * @fn setSpeed1
   * @brief 设置标准风速1
   * 
   * @param 标准风速1数据
   */
  void setSpeed1(float speed);
  /**
   * @fn setSpeed2
   * @brief 设置标准风速2
   * 
   * @param 标准风速2数据
   */
  void setSpeed2(float speed);
  /**
   * @fn calibrationSpeed
   * @brief 开始计算数据
   * @return 状态数据
   */
  String calibrationSpeed(void);
  /**
   * @fn configDTU
   * 
   * @brief 配置DTU是否使能
   * @param dtuswitch DTU开关
   * @param method 工作模式
  */
  uint8_t configDTU(char* dtuswitch, char* method);
  /**
   * @fn configWIFI 
   * 
   * @brief wifi 信息配置
   * @param SSID wifi 名称
   * @param PWD wifi 密码
  */
  uint8_t configWIFI(char* SSID, char* PWD);
  /**
   * @fn configLora
   * 
   * @brief Lora 配置
   * @param DEUI 网关
   * @param EUI 节点
   * @param KEY 密钥
  */
  uint8_t configLora(char* DEUI, char* EUI,char* KEY);
  /**
   * @fn configMQTT1
   * 
   * @brief MQTT配置1
   * @param Server mqtt平台
   * @param Server_IP mqtt平台IP
   * @param Save  传输数据是否保存
  */
  uint8_t configMQTT1(char* Server, char* Server_IP,char* Save);
  /**
   * @fn configMQTT2
   * 
   * @brief configMQTT2 MQTT配置2
   * @param Iot_ID 登录用户名称
   * @param Iot_PWD 登录用户密码
  */
  uint8_t configMQTT2(char* Iot_ID,char* Iot_PWD);
  /**
   * @fn configTopic
   * 
   * @brief 主题订阅
   * @param name 主题名称
   * @param chan 密钥
  */
  uint8_t configTopic(char* name,char* chan);
```

## 兼容性

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |


## 历史

- 2023/06/8 - 1.0.0 版本

## 创作者

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))





