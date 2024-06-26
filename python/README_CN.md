# DFRobot_LarkWeatherStation
* [English Version](./README.md)

这是一个云雀的驱动库，可以用它来获取云雀传感器中的数据。


![产品效果图片](./resources/images/EDU0157.png)


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

```python
def begin(self):
    '''!
      @brief 初始化SCI采集模块，主要用于初始化通信接口
      @return int 初始化状态
      @n       0      初始化成功
      @n      others  初始化失败
    '''
  def get_value(self, keys):
    '''!
      @brief 获取传感器数据
      @param keys  需要获取的数据
      @return 返回获取的数据
    '''

  def get_unit(self, keys):
    '''!
      @brief 获取数据单位
      @param keys  需要获取的数据
      @return 返回获取后的单位
    '''

  def get_information(self, state):
    '''!
      @brief 获取全部数据
      @param state true:加入时间戳 false:不加时间戳
      @return String 返回获取的全部数据
    '''

  def set_time(self, year, month, day,hour, minute, second):
    '''!
      @brief 设置RTC时间
      @param year 年
      @param month 月
      @param day 日
      @param hour 时
      @param minute 分
      @param second 秒
    '''
  def get_time_stamp(self):
    '''!
      @brief 获取RTC时间
    '''
  def config_DTU(self, dtuswitch, method):
    '''!
      @brief 配置DTU启用
      @param dtuswitch DTU开关
      @param method 操作模式
    '''
  def config_WIFI(self,ssid,pwd):
    '''!
      @brief 配置WiFi信息
      @param SSID WiFi名称
      @param PWD WiFi密码
    '''
  def config_Lora(self,deui,eui,key):
    '''!
      @brief 配置LoRa
      @param DEUI 网关
      @param EUI 节点
      @param KEY 密钥
    '''
  def config_MQTT1(self,Server,Server_IP,Save):
    '''!
      @brief MQTT配置1
      @param Server MQTT平台
      @param Server_IP MQTT平台IP
      @param Save 是否保存传输的数据
    '''
  def config_MQTT2(self,Iot_ID,Iot_PWD):
    '''!
      @brief MQTT配置2
      @param Iot_ID 登录用户名
      @param Iot_PWD 登录密码
    '''
  def config_Topic(self,name,chan):
    '''!
      @brief 主题订阅
      @param name 主题名称
      @param chan 关键字
    '''

  
```

## 兼容性

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |     √     |            |          |         |
| RaspberryPi4 |           |            |     √    |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## 历史

- 2023-06-8 - 1.0.0 版本

## 创作者

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))





