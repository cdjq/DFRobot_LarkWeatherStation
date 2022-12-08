#include "DFRobot_Atmospherlum.h"

DFRobot_Atmospherlum::DFRobot_Atmospherlum(uint8_t addr, TwoWire *pWire)
{
  _addr = addr;
  _pWire = pWire;
}

DFRobot_Atmospherlum::DFRobot_Atmospherlum(uint8_t addr, Stream *s)
{
  _addr = addr;
  _s = s;
}

uint8_t DFRobot_Atmospherlum::begin(void)
{
  if(_addr > 0xF7){
    DBG("Invaild Device addr.");
  }
  if(_addr != 0){
    if(!detectDeviceAddress(_addr)){
      DBG("Device addr Error.");
      return 1;
    }
  }else{
    return 1;
  }
  return 0;
 }

bool  DFRobot_Atmospherlum::detectDeviceAddress(uint8_t addr)
{
  if(_pWire){
  uint8_t buf[2];
  _pWire->begin();
  readReg(DEVICE_ADDR, buf, 1,CMD_READ_INPUT);
  DBG((buf[1] << 8| buf[0]));
  if(addr == ((buf[1] << 8| buf[0]) & 0xFF))
    return true;
  }else{
    uint16_t ret = readInputRegister(addr, DEVICE_ADDR);
    
    if((ret & 0xFF) == addr)
      return true;
  }
  return false;
}

uint16_t DFRobot_Atmospherlum::getTemp(void)
{
  uint8_t buf[2];
  readReg(INPUT_TEMP, buf, 1, CMD_READ_INPUT);
  return buf[1] << 8| buf[0];
}

uint16_t DFRobot_Atmospherlum::getHUM(void){
  uint8_t buf[2];
  readReg(INPUT_HUMIDITY, buf, 1, CMD_READ_INPUT);
  return buf[1] << 8| buf[0];
}

float DFRobot_Atmospherlum::getWindSpeed(void)
{
  uint8_t buf[2];
  readReg(INPUT_WIND_SPEED, buf, 1, CMD_READ_INPUT);
  return (float)(buf[1] << 8| buf[0])/100.0;
}

String DFRobot_Atmospherlum::getWindDir(void){
  String str="";
  uint16_t data = 0;
  uint8_t buf[2];
  readReg(INPUT_WIND_DIRECTION, buf, 1, CMD_READ_INPUT);
  data = buf[1] << 8| buf[0];
  switch(data){
        case 0x00:
          str = "N";
        break;
        case 0x2D:
          str = "NE";
        break;
        case 0x5A:
          str = "E";
        break ;
        case 0x87:
          str = "SE";
        break;
        case 0xB4:
          str = "S";
        break;
        case 0xE1:
          str = "SW";
        break;
        case 0x10E:
          str = "W";
        break;
        case 0x13B:
          str = "NW";
        break;
       default:
        break;
      }
  return str;
}

uint32_t DFRobot_Atmospherlum::getAirPressure(void)
{
  uint8_t buf[4];
  readReg(INPUT_PRESSURE_LOW, buf, 2, CMD_READ_INPUT);
  return buf[4] << 24 | buf[3] << 16 | buf[1] << 8 | buf[0];
}

uint16_t DFRobot_Atmospherlum::getAltitude(void)
{
  uint8_t buf[2];
  readReg(INPUT_ALTITUDE, buf, 1, CMD_READ_INPUT);
  return buf[1] << 8| buf[0];
}

sTime_t DFRobot_Atmospherlum::getTime(void)
{
  sTime_t time;
  uint8_t buf[14];
  readReg(HOLDING_YEAR, buf, 7, CMD_READ_HOLDING);
  time.year = buf[1] << 8 | buf[0];
  time.month = buf[3] << 8 | buf[2];
  time.day = buf[5] << 8 | buf[4];
  time.hour = buf[7] << 8 | buf[6];
  time.minute = buf[9] << 8 | buf[8];
  time.second = buf[11] << 8 | buf[10];
  time.week = buf[13] << 8 | buf[12];
  return time;
}

void DFRobot_Atmospherlum::setTime(sTime_t time)
{
  uint8_t buf[14];
  memcpy(buf,(uint8_t*)&time,sizeof(time));
  writeReg(HOLDING_YEAR,buf,7);
}

String DFRobot_Atmospherlum::getInformation(bool state)
{
  sTime_t time;
  String data = "";
  if(state == true){
    time = getTime();
    data += convertHourMinuteSecond(time);
    data += ",";
  }
  data +="WindSpeed: ";
  data += (String)getWindSpeed();
  data +=" m/s,WindDirection:";
  data += (String)getWindDir();
  data += " ,Temp: ";
  data += (String)getTemp();
  data += " C,Humi: ";
  data += (String)getHUM();
  data += " %RH,Pressure: ";
  data += (String)((float)getAirPressure() / 100.0);
  data += " hPa,Altitude: ";
  data += (String)getAltitude();
  data += " m";
  return data;
}

String DFRobot_Atmospherlum::getValue(const char *str)
{
  String data = "Error";
  if(strcmp(str,"Speed") == 0){
    data = (String)getWindSpeed();
  }
  if(strcmp(str,"DIR") == 0){
    data =(String)getWindDir();
  }
  if(strcmp(str,"Temp") == 0){
    data = (String)getTemp();
  }
  if(strcmp(str,"Humi") == 0){
    data =(String)getHUM();
  }
  if(strcmp(str,"Pressure") == 0){
    data = (String)((float)getAirPressure() / 100.0);
  }
  if(strcmp(str,"Altitude") == 0){
    data = (String)getAltitude();
  }
  if(strcmp(str,"Battery") == 0){
    data = (String)getBatteryCapacity();
  }
  return data;
}
String DFRobot_Atmospherlum::getUnit(const char *str)
{
  String data = "Error";
  if(strcmp(str,"Speed") == 0){
    data = " m/s";
  }
  if(strcmp(str,"Temp") == 0){
    data = " C";
  }
  if(strcmp(str,"Humi") == 0){
    data = " %RH";
  }
  if(strcmp(str,"Pressure") == 0){
    data = " hPa";
  }
  if(strcmp(str,"Altitude") == 0){
    data = " m";
  }
  if(strcmp(str,"Battery") == 0){
    data = " %";
  }
  return data;
}
String DFRobot_Atmospherlum::getTimeStamp()
{
  sTime_t time;
  time = getTime();
  return convertHourMinuteSecond(time);
}

uint16_t DFRobot_Atmospherlum::getBatteryCapacity(void)
{
   uint8_t buf[2];
  readReg(INPUT_BATTERYVALUE, buf, 1, CMD_READ_INPUT);
  return buf[1] << 8| buf[0];
}
String DFRobot_Atmospherlum::convertHourMinuteSecond(sTime_t t){
    String rlt = "";
    if(t.hour < 10) rlt += '0';
    rlt += String(t.hour) + ':';
    if(t.minute < 10) rlt += '0';
    rlt += String(t.minute) + ':';
    if(t.second < 10) rlt += '0';
    rlt += String(t.second);
    return rlt;
}

uint8_t DFRobot_Atmospherlum::readReg(uint16_t reg, void *pBuf, uint8_t size,uint8_t stateReg)
{
  uint8_t* _pBuf = (uint8_t*)pBuf;
  uint8_t _reg  = 0;
    if(pBuf == NULL){
      DBG("data error");
      return 0;
    }
  if(_pWire){
    _pWire->beginTransmission(_addr);
    _pWire->write(_addr);
    _pWire->write(stateReg);
    _pWire->write(0);
    _pWire->write(reg);
    _pWire->write(0);
    _pWire->write(size);
    _pWire->endTransmission();
    _pWire->requestFrom(_addr, size*2);
    for(uint8_t i = 0; i < size*2; i++)
      _pBuf[i] = _pWire->read();
    return size;
  }else{
    return readInputRegister(_addr, reg, _pBuf, size);
  }
}
uint8_t DFRobot_Atmospherlum::writeReg(uint8_t reg, void *pBuf, size_t size)
{
  uint8_t *_pBuf = (uint8_t*)pBuf;

  uint8_t ret = 0;
  if(_pWire){
    _pWire->beginTransmission(_addr);
    _pWire->write(_addr);
    _pWire->write(CMD_WRITE_MULTI_HOLDING);
    _pWire->write(0);
    _pWire->write(reg);
    _pWire->write(0);
    _pWire->write(size);
    _pWire->write(size * 2);
    for(uint8_t i = 0; i < size * 2; i++){
      _pWire->write(_pBuf[i]);
    }
    _pWire->endTransmission();
  }else{
    ret = writeHoldingRegister(_addr,reg,_pBuf,size);
  }
  return ret;
}