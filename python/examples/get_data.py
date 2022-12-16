from __future__ import print_function
import sys
import os
sys.path.append("../")
import time
import RPi.GPIO as GPIO

from DFRobot_Atmospherlum import *

ctype=0

ADDRESS = 0x42 
I2C_1   = 0x01


if ctype==0:
  EDU0157 = DFRobot_Atmospherlum_I2C(I2C_1 ,ADDRESS)
else:
  EDU0157 = DFRobot_Atmospherlum_UART(9600, ADDRESS)


def setup():
  while (EDU0157.begin() == False):
    print("Sensor initialize failed!!")
    time.sleep(1)
  print("Sensor initialize success!!")
  EDU0157.set_time(2022,12,14,10,0,0,3)
  time.sleep(1)
  
  
def loop():
  print("------------------")
  print("Speed=",EDU0157.get_value("Speed"),EDU0157.get_unit("Speed"))
  print("DIR=",EDU0157.get_value("DIR"))
  print("Temp=",EDU0157.get_value("Temp"),EDU0157.get_unit("Temp"))
  print("Humi=",EDU0157.get_value("Humi"),EDU0157.get_unit("Humi"))
  print("Pressure=",EDU0157.get_value("Pressure"),EDU0157.get_unit("Pressure"))
  print("Altitude=",EDU0157.get_value("Altitude"),EDU0157.get_unit("Altitude"))
  print("Battery=",EDU0157.get_value("Battery"),EDU0157.get_unit("Battery"))
  print(EDU0157.get_information(True))
  print(EDU0157.get_information())
  print(EDU0157.get_time_stamp())
  time.sleep(1)

if __name__ == "__main__":
  setup()
  while True:
    loop()