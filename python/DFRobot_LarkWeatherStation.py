# -*- coding: utf-8 -*-
import serial
import time
import smbus
import os
import math
import RPi.GPIO as GPIO
import math
import serial

I2C_MODE                  = 0x01
UART_MODE                 = 0x02
DEV_ADDRESS               = 0x42


class DFRobot_LarkWeatherStation:

  DEBUG_TIMEOUT_MS  =  2

  CMD_GET_DATA            =    0x00 #根据传过来的名称返回名字
  CMD_GET_ALL_DATA         =   0x01 #获取板载全部传感器数据
  CMD_SET_TIME             =   0x02 #设置板载RTC时间
  CME_GET_TIME             =   0x03
  CMD_GET_UNIT             =   0x04 #获取传感器单位
  CMD_GET_VERSION          =   0x05 #获取版本号
  CMD_RESET_DATA           =   0x06
  CMD_RADIUS_DATA          =    0x07#设置风杯半径
  CMD_SPEED1_DATA          =   0x08#设置标准风速1
  CMD_SPEED2_DATA          =   0x09#设置标准风速2
  CMD_CALIBRATOR           =   0x0a#开始校准计算

  IIC_MAX_TRANSFER         =   32    #Maximum transferred data via I2C
  I2C_ACHE_MAX_LEN         =   32
  CMD_END         =    CMD_CALIBRATOR


  ERR_CODE_NONE           =    0x00 #Normal communication 
  ERR_CODE_CMD_INVAILED    =   0x01 #Invalid command
  ERR_CODE_RES_PKT         =   0x02 #Response packet error
  ERR_CODE_M_NO_SPACE      =   0x03 #Insufficient memory of I2C controller(master)
  ERR_CODE_RES_TIMEOUT     =   0x04 # Response packet reception timeout
  ERR_CODE_CMD_PKT         =   0x05 #Invalid command packet or unmatched command
  ERR_CODE_SLAVE_BREAK      =  0x06 #Peripheral(slave) fault
  ERR_CODE_ARGS             =  0x07 # Set wrong parameter
  ERR_CODE_SKU              =  0x08 # The SKU is an invalid SKU, or unsupported by SCI Acquisition Module
  ERR_CODE_S_NO_SPACE       =  0x09 # Insufficient memory of I2C peripheral(slave)
  ERR_CODE_I2C_ADRESS      =   0x0A # Invalid I2C address

  STATUS_SUCCESS   =   0x53  #Status of successful response   
  STATUS_FAILED     =  0x63  # Status of failed response 

  INDEX_CMD        = 0
  INDEX_ARGS_NUM_L = 1
  INDEX_ARGS_NUM_H = 2
  INDEX_ARGS       = 3

  INDEX_YEAR     = 0
  INDEX_MONTH    = 1
  INDEX_DAY      = 2
  INDEX_WEEK     = 3
  INDEX_HOUR     = 4
  INDEX_MINUTE   = 5
  INDEX_SECOND   = 6

  INDEX_RES_ERR    = 0
  INDEX_RES_STATUS = 1
  INDEX_RES_CMD    = 2
  INDEX_RES_LEN_L  = 3
  INDEX_RES_LEN_H  = 4
  INDEX_RES_DATA   = 5
  def __init__(self):
    pass
  
  def begin(self):
    '''!
      @brief 初始化SCI采集模块，主要用于初始化通信接口
      @return int 初始化状态
      @n       0      初始化成功
      @n      others  初始化失败
    '''
    return 0
  def get_value(self, keys):
    '''!
      @brief 获取传感器数据
      @param keys  需要获取的数据
      @return 返回获取的数据
    '''
    rslt = ""
    length = len(keys)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_GET_DATA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in keys:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    #time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_GET_DATA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
      if length:
        for data in recv_pkt[self.INDEX_RES_DATA:]:
          rslt += chr(data)
    return rslt

  def get_unit(self, keys):
    '''!
      @brief 获取数据单位
      @param keys  需要获取的数据
      @return 返回获取后的单位
    '''
    rslt = ""
    length = len(keys)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_GET_UNIT
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in keys:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    #time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_GET_UNIT)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
      if length:
        for data in recv_pkt[self.INDEX_RES_DATA:]:
          rslt += chr(data)
    return rslt
  
  def set_radius(self,radius):
    '''!
      @brief 设置风杯半径
      @param radius 半径
    '''
    length = 2
    data = radius * 100
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_RADIUS_DATA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    pkt[self.INDEX_ARGS + 0]       = data >> 8
    pkt[self.INDEX_ARGS + 1]       = data & 0xff
    self._send_packet(pkt)
    time.sleep(2)
    recv_pkt = self._recv_packet(self.CMD_RADIUS_DATA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
      return 1
    
  def get_information(self, state):
    '''!
      @brief 获取全部数据
      @param state true:加入时间戳 false:不加时间戳
      @return String 返回获取的全部数据
    '''
    rslt = ""
    length = 1
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_GET_ALL_DATA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    if state == True :
      pkt[self.INDEX_ARGS]       = 1
    else:
      pkt[self.INDEX_ARGS]       = 0
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_GET_ALL_DATA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
      if length:
        for data in recv_pkt[self.INDEX_RES_DATA:]:
          rslt += chr(data)
    return rslt

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
    length = 7
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_SET_TIME
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    pkt[self.INDEX_ARGS + self.INDEX_YEAR]       = (year - 2000)
    pkt[self.INDEX_ARGS + self.INDEX_MONTH]       = month
    pkt[self.INDEX_ARGS + self.INDEX_DAY]       = day
    pkt[self.INDEX_ARGS + self.INDEX_WEEK]       = 0
    pkt[self.INDEX_ARGS + self.INDEX_HOUR]       = hour
    pkt[self.INDEX_ARGS + self.INDEX_MINUTE]       = minute
    pkt[self.INDEX_ARGS + self.INDEX_SECOND]       = second
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_SET_TIME)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)

  def get_time_stamp(self):
    '''!
      @brief 获取RTC时间
    '''
    rslt = ""
    length = 0
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CME_GET_TIME
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CME_GET_TIME)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
      if length:
        for data in recv_pkt[self.INDEX_RES_DATA:]:
          rslt += chr(data)
    return rslt

  def _recv_packet(self, cmd):
    '''!
      @brief Receive and parse the response data packet
      @param cmd Command to receive the packet
      @return Error code and response packet list
      @n      The zeroth element in the list: error code, only when the error code is ERR_CODE_NONE, there can be other elements
      @n      The first element in the list: response packet status code, 0x53-correct response packet 0x63-wrong response packet
      @n      The second element in the list: response packet command, which indicates the response packet belongs to which communication command
      @n      The third element in the list: low byte of the valid data length after the response packet
      @n      The fourth element in the list: high byte of the valid data length after the response packet
      @n      The 5th element or more in the list: valid data
    '''
    rslt = [0] * 1
    t = time.time()
    while time.time() - t < self.DEBUG_TIMEOUT_MS:
      status = self._recv_data(1)[0]
      #print(status)
      if status != 0xff:
        if status == 0xD3:
          self._reset_data()
          time.sleep(0.2)
        else:
          if status == self.STATUS_SUCCESS or status == self.STATUS_FAILED:
            command = self._recv_data(1)[0]
            #print("command=%x cmd=%x"%(command,cmd))
            if command != cmd:
              rslt[0] = self.ERR_CODE_RES_PKT
              print("Response pkt is error!")
              return rslt
            lenL = self._recv_data(2)
            length = (lenL[1] << 2) | lenL[0]
            #print("length=%x length=%d"%(length,length))
            rslt[0] = self.ERR_CODE_NONE
            rslt = rslt + [status, command, lenL[0], lenL[1]]
            if length:
              rslt = rslt + self._recv_data(length)
            #print(rslt)
            #print("time: %f"%(time.time() - t))
            return rslt
      time.sleep(0.1)
    print("time out: %f"%(time.time() - t))
    return [self.ERR_CODE_RES_TIMEOUT]
    
  def _reset_data(self):
    length = 0
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_RESET_DATA
    pkt[self.INDEX_ARGS_NUM_L] = 0
    pkt[self.INDEX_ARGS_NUM_H] = 0
    self._send_packet(pkt)
    time.sleep(0.1)
    


class DFRobot_LarkWeatherStation_I2C(DFRobot_LarkWeatherStation):
  def __init__(self,addr):
    '''!
      @brief DFRobot_SCI_IIC Constructor
      @param addr:  7-bit IIC address, support the following address settings
      @n RP2040_SCI_ADDR_0X21      0x21 default I2C address
      @n RP2040_SCI_ADDR_0X22      0x22
      @n RP2040_SCI_ADDR_0X23      0x23
    '''
    self._addr = addr
    self._bus = smbus.SMBus(1)
    DFRobot_LarkWeatherStation.__init__(self)
    

  def _send_packet(self, pkt):
    '''!
      @brief Send data
      @param pkt List of data to be sent
      @return None
    '''
    for data in pkt:
      try:
        self._bus.write_byte(self._addr, data)
      except:
        pass
    
  def _recv_data(self, len):
    '''!
      @brief Read data
      @param len Number of bytes to be read
      @return The read data list
    '''
    rslt = [0]*len
    i = 0
    while i < len:
      try:
        rslt[i] = self._bus.read_byte(self._addr)
      except:
        rslt[i] = 0
      i += 1
    return rslt

class DFRobot_LarkWeatherStation_UART(DFRobot_LarkWeatherStation):
  def __init__(self):
    '''!
      @brief DFRobot_SCI_IIC Constructor
      @param addr:  7-bit IIC address, support the following address settings
      @n RP2040_SCI_ADDR_0X21      0x21 default I2C address
      @n RP2040_SCI_ADDR_0X22      0x22
      @n RP2040_SCI_ADDR_0X23      0x23
    '''
    self.ser = serial.Serial("/dev/ttyAMA0",115200)
    if self.ser.isOpen == False:
      self.ser.open()
    DFRobot_LarkWeatherStation.__init__(self)
  def _send_packet(self, pkt):
    '''!
      @brief Send data
      @param pkt List of data to be sent
      @return None
    '''
    self.ser.write(pkt)
    
  def _recv_data(self, len):
    '''!
      @brief Read data
      @param len Number of bytes to be read
      @return The read data list
    '''
    
    rslt = [0]*len
    i = 0
    while i < len:
      try:
        rslt[i] =  ord(self.ser.read(1))
      except:
        rslt[i] = 0
      i += 1
    return rslt