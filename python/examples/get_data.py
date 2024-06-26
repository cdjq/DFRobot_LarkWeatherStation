# -*- coding: utf-8 -*-
'''!
  @file getData.ino
  @brief This is a routine to get skylark data
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
  EDU0157.set_time(2023,1,11,23,59,0)
  time.sleep(1)
  
  
def loop():
  print("------------------")
  print("Speed=",EDU0157.get_value("speed"),EDU0157.get_unit("speed"))
  print("DIR=",EDU0157.get_value("dir"))
  print("Temp=",EDU0157.get_value("Temp"),EDU0157.get_unit("Temp"))
  print("Humi=",EDU0157.get_value("Humi"),EDU0157.get_unit("Humi"))
  print("Temp=",EDU0157.get_value("Pressure"),EDU0157.get_unit("Pressure"))
  print("Humi=",EDU0157.get_value("Altitude"),EDU0157.get_unit("Altitude"))
  print(EDU0157.get_information(True))
  time.sleep(1)

if __name__ == "__main__":
  setup()
  while True:
    loop()