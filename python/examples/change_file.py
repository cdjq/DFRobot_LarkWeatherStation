# -*- coding: utf-8 -*-
'''!
  @file change_file.ino
  @brief This is a routine to modify the DTU file
  @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license      The MIT License (MIT)
  @author       [TangJie](jie.tang@dfrobot.com)
  @version      V1.0.0
  @date         2023-06-8
  @url         https://github.com/DFRobot/DFRobot_LarkWeatherStation
'''
from __future__ import print_function
import sys
import os
sys.path.append("../")
import time

from DFRobot_Atmospherlum import *

ctype=0

ADDRESS = 0x42 


EDU0157 = DFRobot_Atmospherlum_I2C(ADDRESS)
#EDU0157 = DFRobot_Atmospherlum_UART()
def setup():
  while EDU0157.begin() != 0:
    print("Sensor initialize failed!!")
    time.sleep(1)
  print("Sensor initialize success!!")
  while True:
    if EDU0157.config_DTU("off","wifi") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_WIFI("AAAA","BBBB") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_Lora("AAAA","BBBB","aaaa") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_Lora("AAAA","BBBB","aaaa") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_MQTT1("aaa","BBBB","aaaa") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_MQTT2("aaa","BBBB") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_Topic("Topic_Humi","BBBB") == 1:
      break
    time.sleep(1)
  while True:
    if EDU0157.config_Topic("Topic_kk","BBBB") == 1:
      break
    time.sleep(1)
  print("change ok");
  
  
def loop():
  
  time.sleep(1)

if __name__ == "__main__":
  setup()
  while True:
    loop()