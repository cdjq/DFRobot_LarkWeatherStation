import serial
import time
import smbus
import os
import math
import RPi.GPIO as GPIO
import math

import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu


CMD_READ_HOLDING           =  0x03
CMD_READ_INPUT             =  0x04
CMD_WRITE_MULTI_HOLDING    =  0x10
DEVICE_ADDR                =  0x02
INPUT_WIND_SPEED           =  0x06
INPUT_WIND_DIRECTION       =  0x07
INPUT_TEMP                 =  0x08
INPUT_HUMIDITY             =  0x09
INPUT_PRESSURE_LOW         =  0x0A
INPUT_ALTITUDE             =  0x0C
INPUT_BATTERYVALUE         =  0x0D

HOLDING_STORAGETIME        =  0x07
HOLDING_YEAR               =  0x08
HOLDING_MONTH              =  0x09
HOLDING_DAY                =  0x0A
HOLDING_HOUR               =  0x0B
HOLDING_MINUTE             =  0x0C
HOLDING_SECOND             =  0x0D
HOLDING_WEEK               =  0x0E
HOLDING_POWER              =  0x0F

I2C_MODE                  = 0x01
UART_MODE                 = 0x02
DEV_ADDRESS               = 0x42

class DFRobot_Atmospherlum():
  def __init__(self ,bus = 0 ,baud = 9600, mode = I2C_MODE):
    self.mode = 0
    self.resolution = 0
    self.gain = 0
    if mode == I2C_MODE:
      self.i2cbus = smbus.SMBus(bus)
      self._uart_i2c = I2C_MODE
    else:
      self.master = modbus_rtu.RtuMaster(serial.Serial(port="/dev/ttyAMA0",baudrate=baud, bytesize=8, parity='N', stopbits=1))
      self.master.set_timeout(1.0)
      self._uart_i2c = UART_MODE

  def _detect_device_address(self):
    '''!
      @brief Get sensor address
      @return  Return sensor address
    '''
    if self._uart_i2c == I2C_MODE:
      rbuf = self._read_reg(DEVICE_ADDR,1,CMD_READ_INPUT)
    else:
      rbuf = self._read_reg(0x02,1)
    if self._uart_i2c == I2C_MODE:
      data = rbuf[1] << 8 | rbuf[0]
    elif self._uart_i2c == UART_MODE:
      data = rbuf[0]
    return data

  def begin(self):
    '''
      @brief 初始化传感器
    '''
    if self._detect_device_address() != DEV_ADDRESS:
      return False
    return True
    
  def set_time(self,year,month,day,hour,minute,second,week):
    buffer = [(year  & 0xff),(year>>8) & 0xff,month,0,day,0,hour,0,minute,0,second,0,week,0]
    self._write_reg(0x55,buffer)

  def get_value(self,strname):
    returnstr = "errot"
    if strname == "Speed":
      returnstr = str(self._get_wind_speed())
    if strname == "DIR":
      returnstr = str(self._get_wind_dir())
    if strname == "Temp":
      returnstr = str(self._get_temp())
    if strname == "Humi":
      returnstr = str(self._get_hum())
    if strname == "Pressure":
      returnstr = str(self._get_air_pressure())
    if strname == "Altitude":
      returnstr = str(self._get_altitude())
    if strname == "Battery":
      returnstr = str(self._get_batterycapacity())
    return returnstr
  
  def get_unit(self,strname):
    returnstr = "errot"
    if strname == "Speed":
      returnstr = " m/s"
    if strname == "Temp":
      returnstr = " C"
    if strname == "Humi":
      returnstr = " %RH"
    if strname == "Pressure":
      returnstr = " hPa"
    if strname == "Altitude":
      returnstr = "m"
    if strname == "Battery":
      returnstr = "%"
    return returnstr

  def get_information(self,state=False):
    returnstr = ""
    if state == True:
      returnstr += self._gettime()
      returnstr += ","
    returnstr +="WindSpeed: "
    returnstr +=  str(self._get_wind_speed())
    returnstr +=" m/s,WindDirection:"
    returnstr += str(self._get_wind_dir())
    returnstr += " ,Temp: "
    returnstr +=  str(self._get_temp())
    returnstr += " C,Humi: "
    returnstr += str(self._get_hum())
    returnstr += " %RH,Pressure: "
    returnstr += str(self._get_air_pressure())
    returnstr += " hPa,Altitude: "
    returnstr += str(self._get_altitude())
    returnstr += " m"
    return returnstr
    
  def get_time_stamp(self):
    return self._gettime()

  def _gettime(self):
    buf = self._read_reg(HOLDING_YEAR+0x21,8,CMD_READ_HOLDING)
    year = buf[1]<<8|buf[0]
    month = buf[3]<<8|buf[2]
    day = buf[5]<<8|buf[4]
    hour = buf[7]<<8|buf[6]
    minute = buf[9]<<8|buf[8]
    second = buf[11]<<8|buf[10]
    week = buf[13]<<8|buf[12]
    data = ""
    if hour < 10:
      data += '0'
    data += str(hour)
    data += ':'
    if minute < 10:
      data += '0'
    data += str(minute)
    data += ':'
    if second < 10:
      data += '0'
    data += str(second)
    return data

  def _get_temp(self):
    buf = self._read_reg(INPUT_TEMP,1,CMD_READ_INPUT)
    return buf[1]<<8|buf[0]

  def _get_hum(self):
    buf = self._read_reg(INPUT_HUMIDITY,1,CMD_READ_INPUT)
    return buf[1]<<8|buf[0]

  def _get_wind_speed(self):
    buf = self._read_reg(INPUT_WIND_SPEED,1,CMD_READ_INPUT)
    return float(buf[1]<<8|buf[0])/100.0

  def _get_wind_dir(self):
    buf = self._read_reg(INPUT_WIND_DIRECTION,1,CMD_READ_INPUT)
    str = ""
    data = buf[1]<<8|buf[0]
    if data == 0x00:
      str = "N"
    elif data == 0x2D:
      str = "NE"
    elif data == 0x5A:
      str = "E"
    elif data == 0x87:
      str = "SE"
    elif data == 0xB4:
      str = "S"
    elif data == 0xE1:
      str = "SW"
    elif data == 0x10E:
      str = "W"
    elif data == 0x13B:
      str = "NW"  
    return str

  def _get_air_pressure(self):
    buf = self._read_reg(INPUT_PRESSURE_LOW,2,CMD_READ_INPUT)
    return (buf[2] << 24 | buf[3] << 16 | buf[1] << 8 | buf[0])/100

  def _get_altitude(self):
    buf = self._read_reg(INPUT_ALTITUDE,1,CMD_READ_INPUT)
    return buf[1] << 8| buf[0]
    
  def _get_batterycapacity(self):
    buf = self._read_reg(INPUT_BATTERYVALUE,1,CMD_READ_INPUT)
    return buf[1] << 8| buf[0]

class DFRobot_Atmospherlum_I2C(DFRobot_Atmospherlum):
  '''!
    @brief An example of an i2c interface module
  '''
  def __init__(self ,bus ,addr):
    self._addr = addr
    DFRobot_Atmospherlum.__init__(self,bus,0,I2C_MODE)   
    
  
  def _read_reg(self, reg_addr ,length,state):
    '''!
      @brief read the data from the register
      @param reg_addr register address
      @param length read data
    '''
    # buffer = [state,0,reg_addr,0,length]
    # self.i2cbus.write_i2c_block_data(self._addr ,0x42 , buffer)
    rslt = self.i2cbus.read_i2c_block_data(self._addr ,reg_addr, length*2)
    return rslt

  def _write_reg(self, reg_addr ,data):
    '''!
      @brief write the data from the register
      @param reg_addr register address
      @param data 写入寄存器数据
    '''
    try:
      rslt = self.i2cbus.write_i2c_block_data(self._addr ,reg_addr , data)
    except:
      rslt = -1
    return rslt        


class DFRobot_Atmospherlum_UART(DFRobot_Atmospherlum):
  '''!
    @brief An example of an UART interface module
  '''
  def __init__(self ,baud, addr):
    self._baud = baud
    self._addr = addr
    try:
      DFRobot_Atmospherlum.__init__(self,0,self._baud,UART_MODE)
    except:
      print ("plese get root!")
   
  
  def _read_reg(self, reg_addr ,length):
    '''!
      @brief Read data from the sensor
    '''
    return list(self.master.execute(self._addr, cst.READ_INPUT_REGISTERS, reg_addr, length))
  
  def _write_reg(self, reg_addr ,data):
    '''!
      @brief write data from the sensor
    '''
    return list(self.master.execute(self._addr, cst.WRITE_MULTIPLE_REGISTERS, reg_addr, output_value=data))