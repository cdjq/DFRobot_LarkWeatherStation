# -*- coding: utf-8 -*-
'''!
  @file       DFRobot_LarkWeatherStation.py
  @brief       DFRobot_LarkWeatherStation Class infrastructure
  @copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT License (MIT)
  @author      TangJie(jie.tang@dfrobot.com)
  @version     V1.0
  @date        2023-07-03
  @url         https://github.com/DFRobor/DFRobot_LarkWeatherStation
'''
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


class DFRobot_Atmospherlum:

  DEBUG_TIMEOUT_MS  =  4

  CMD_GET_DATA            =    0x00 #Return the name based on the given name
  CMD_GET_ALL_DATA         =   0x01 #Get all onboard sensor data
  CMD_SET_TIME             =   0x02 #Set onboard RTC time
  CME_GET_TIME             =   0x03
  CMD_GET_UNIT             =   0x04 #Get sensor units
  CMD_GET_VERSION          =   0x05 #Get version number
  CMD_RESET_DATA           =   0x06
  CMD_RADIUS_DATA     =   0x07  # Set the radius of the anemometer cup
  CMD_SPEED1_DATA     =   0x08  # Set standard wind speed 1
  CMD_SPEED2_DATA     =   0x09  # Set standard wind speed 2
  CMD_CALIBRATOR      =   0x0a  # Start calibration calculation

  CMD_DTU             =   0x0c  # DTU configuration
  CMD_WIFI            =   0x0d  # WiFi configuration
  CMD_LORA            =   0x0e  # LoRa configuration
  CMD_MQTT1           =   0x10  # MQTT configuration 1
  CMD_MQTT2           =   0x11  # MQTT configuration 2
  CMD_TOP             =   0x12  # Topic configuration

  IIC_MAX_TRANSFER         =   32    #Maximum transferred data via I2C
  I2C_ACHE_MAX_LEN         =   32
  CMD_END         =    CMD_TOP


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
      @brief Initialize the SCI acquisition module, mainly used for initializing the communication interface
      @return int Initialization status
      @n       0      Initialization successful
      @n      others  Initialization failed
    '''
    return 0
  def get_value(self, keys):
    '''!
      @brief Get sensor data
      @param keys  Data to be obtained
      @return Returns the acquired data
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
      @brief Get data units
      @param keys  Data for which units need to be obtained
      @return Returns the obtained units
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
      @brief Set standard wind speed 1. 
      @param speed Data for standard wind speed 1
    '''
    length = 2
    data = radius * 100;
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
    
  def set_speed1(self,speed1):
    '''!
      @brief Set standard wind speed 2.
      @param speed Data for standard wind speed 2
    '''
    length = 2
    data = speed1 * 100;
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_SPEED1_DATA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    pkt[self.INDEX_ARGS + 0]       = data >> 8
    pkt[self.INDEX_ARGS + 1]       = data & 0xff
    self._send_packet(pkt)
    time.sleep(10)
    recv_pkt = self._recv_packet(self.CMD_SPEED1_DATA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
    
  def set_speed2(self,speed2):
    '''!
      @brief Start calculating data.
      @return Status data
    '''
    length = 2
    data = speed2 * 100;
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_SPEED2_DATA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    pkt[self.INDEX_ARGS + 0]       = data >> 8
    pkt[self.INDEX_ARGS + 1]       = data & 0xff
    self._send_packet(pkt)
    time.sleep(10)
    recv_pkt = self._recv_packet(self.CMD_SPEED2_DATA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)
    
  def calibration_speed(self):
    '''!
      @brief Configure DTU enablement.
      @param dtuswitch DTU switch
      @param method Operation mode
    '''
    length = 0
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_CALIBRATOR
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    self._send_packet(pkt)
    time.sleep(10)
    recv_pkt = self._recv_packet(self.CMD_CALIBRATOR)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      length = recv_pkt[self.INDEX_RES_LEN_L] | (recv_pkt[self.INDEX_RES_LEN_H] << 8)

  def get_information(self, state):
    '''!
      @brief Get all data
      @param state true: include timestamp, false: do not include timestamp
      @return String Returns all the acquired data
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
      @brief Set the RTC time
      @param year Year
      @param month Month
      @param day Day
      @param hour Hour
      @param minute Minute
      @param second Second
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
      @brief Get the RTC time
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
  
  def config_DTU(self, dtuswitch, method):
    '''!
      @brief Configure DTU enablement.
      @param dtuswitch DTU switch
      @param method Operation mode
    '''
    data = dtuswitch + ',' + method
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_DTU
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_DTU)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0
  
  def config_WIFI(self,ssid,pwd):
    '''!
      @brief Configure WiFi information.
      @param SSID WiFi name
      @param PWD WiFi password
    '''
    data = ssid + ',' + pwd
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_WIFI
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_WIFI)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0
  
  def config_Lora(self,deui,eui,key):
    '''!
      @brief Configure LoRa.
      @param DEUI Gateway
      @param EUI Node
      @param KEY Key
    '''
    data = deui + ',' + eui + ',' + key
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_LORA
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_LORA)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0
  
  def config_MQTT1(self,Server,Server_IP,Save):
    '''!
      @brief MQTT configuration 1.
      @param Server MQTT platform
      @param Server_IP MQTT platform IP
      @param Save Whether to save transmitted data
    '''
    data = Server + ',' + Server_IP + ',' +  Save
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_MQTT1
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_MQTT1)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0
  def config_MQTT2(self,Iot_ID,Iot_PWD):
    '''!
      @brief MQTT configuration 2.
      @param Iot_ID Login username
      @param Iot_PWD Login password
    '''
    data = Iot_ID + ',' + Iot_PWD
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_MQTT2
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_MQTT2)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0
  
  def config_Topic(self,name,chan):
    '''!
      @brief Topic subscription.
      @param name Topic name
      @param chan Key
    '''
    data = name + ':' + chan
    length = len(data)
    pkt = [0] * (3 + length)
    pkt[self.INDEX_CMD]        = self.CMD_TOP
    pkt[self.INDEX_ARGS_NUM_L] = length & 0xFF
    pkt[self.INDEX_ARGS_NUM_H] = (length >> 8) & 0xFF
    i = 0
    for c in data:
      pkt[self.INDEX_ARGS + i] = ord(c)
      i += 1
    self._send_packet(pkt)
    time.sleep(0.1)
    recv_pkt = self._recv_packet(self.CMD_TOP)
    if (len(recv_pkt) >= 5) and (recv_pkt[self.INDEX_RES_ERR] == self.ERR_CODE_NONE and recv_pkt[self.INDEX_RES_STATUS] == self.STATUS_SUCCESS):
      return 1
    return 0

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
              #print("Response pkt is error!")
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
    


class DFRobot_Atmospherlum_I2C(DFRobot_Atmospherlum):
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
    DFRobot_Atmospherlum.__init__(self)
    

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

class DFRobot_Atmospherlum_UART(DFRobot_Atmospherlum):
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
    DFRobot_Atmospherlum.__init__(self)
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