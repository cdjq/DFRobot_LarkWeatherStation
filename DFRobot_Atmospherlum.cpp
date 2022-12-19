#include "DFRobot_Atmospherlum.h"

uint16_t skuData[]={0x40E4,0x4211,0x41cc,0x809D,0x416c,0x4202,0x4218,0x4142,0x4141};

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
  readReg(0x29, buf, 7, CMD_READ_HOLDING);
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
  data += getExtendData();
  return data;
}

String DFRobot_Atmospherlum::getExtendData(void){
  uint8_t buf[20];
  String str = "";
  uint8_t dataNumber = 0;
  uint16_t sku1,sku2;
  readReg(INPUT_BOARDSKU1, buf, 10, CMD_READ_INPUT);
  sku1 = buf[1]<<8|buf[0];
  if(sku1 != 0){
    str += getDataTitel(sku1,buf);
  }
  readReg(INPUT_BOARDSKU2, buf, 10, CMD_READ_INPUT);
  sku2 = buf[1]<<8|buf[0];
  if(sku2 != 0){
    str += getDataTitel(sku2,buf);
  }
  return str;
}

String DFRobot_Atmospherlum::getDataTitel(uint16_t sku,void* buf){
  String str = "";
  uint8_t* _buf = (uint8_t*)buf;
  uint32_t lux = 0;
  uint16_t data1,data2,data3,data4,data5,data6,data7,data8;
  if(sku == skuData[0]){
    str += ",Light(lx): ";
    data1= _buf[5]<<8|_buf[4];
    data2= _buf[7]<<8|_buf[6];
    lux  = data1 | data2 <<16;
    lux = lux * (1.0023f + lux * (8.1488e-5f + lux * (-9.3924e-9f + lux * 6.0135e-13f)));
    str += String(lux);
  }else if(sku == skuData[1]){
    data1 = _buf[5]<<8|_buf[4];
    data2 = _buf[7]<<8|_buf[6];
    data3 = _buf[9]<<8|_buf[8];
    data4 = _buf[11]<<8|_buf[10];
    str += ",Angle_N: ";
    str += String(data1);
    str += ",Mag_X(uT): ";
    str += String(data2);
    str += ",Mag_Y(uT): ";
    str += String(data3);
    str += ",Mag_Z(uT): ";
    str += String(data4);
  }else if(sku == skuData[2]){
    data1= _buf[5]<<8|_buf[4];
    data2= _buf[7]<<8|_buf[6];
    data3= _buf[9]<<8|_buf[8];
    str += ",PM1.0(ug/m3): ";
    str += String(data1);
    str += ",PM2.5(ug/m3): ";
    str += String(data2);
    str += ",PM10(ug/m3): ";
    str += String(data3);
  }else if(sku == skuData[3]){
    data1= _buf[5]<<8|_buf[4];
    data2= _buf[7]<<8|_buf[6];
    data3= _buf[9]<<8|_buf[8];
    str += ",Lat: ";
    str += String(data1);
    str += ",Lon: ";
    str += String(data2);
    str += ",Altitude(n): ";
    str += String(data3);
  }else if(sku == skuData[4]){
    data1= _buf[5]<<8|_buf[4];
    data2= _buf[7]<<8|_buf[6];
    data3= _buf[9]<<8|_buf[8];
    data4= _buf[11]<<8|_buf[10];
    data5= _buf[13]<<8|_buf[12];
    data6= _buf[15]<<8|_buf[14];
    data7= _buf[17]<<8|_buf[16];
    data8= _buf[19]<<8|_buf[18];
    str += ",405-425nm: ";
    str += String(data1);
    str += ",435-455nm: ";
    str += String(data2);
    str += ",470-490nm: ";
    str += String(data3);
    str += ",505-525nm: ";
    str += String(data4);
    str += ",545-565nm: ";
    str += String(data5);
    str += ",580-600nm: ";
    str += String(data6);
    str += ",620-640nm: ";
    str += String(data7);
    str += ",670-690nm: ";
    str += String(data8);
  }else if(sku == skuData[5]){
    data1= _buf[5]<<8|_buf[4];
    data2= _buf[7]<<8|_buf[6];
    data3= _buf[9]<<8|_buf[8];
    str += ",AQI: ";
    str += String(data1);
    str += ",TVOC: ";
    str += String(data2);
    str += ",ECO2: ";
    str += String(data3);
  }else if(sku == skuData[6]){
    str += ",CO2(ppm)";
    str += String(_buf[5]<<8|_buf[4]);
  }else if(sku == skuData[7]){
    str += ",O2(%vol)";
    str += String(_buf[5]<<8|_buf[4]);
  }else if(sku == skuData[8]){
    str += ",O3(ppb)";
    str += String(_buf[5]<<8|_buf[4]);
  }
  return str;
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
    _pWire->write(reg);
    // _pWire->write(stateReg);
    // _pWire->write(0);
    // _pWire->write(reg);
    // _pWire->write(0);
    // _pWire->write(size);
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
    _pWire->write(0x55);
    // _pWire->write(CMD_WRITE_MULTI_HOLDING);
    // _pWire->write(0);
    // _pWire->write(reg);
    // _pWire->write(0);
    // _pWire->write(size);
    // _pWire->write(size * 2);
    for(uint8_t i = 0; i < size * 2; i++){
      _pWire->write(_pBuf[i]);
    }
    _pWire->endTransmission();
  }else{
    ret = writeHoldingRegister(_addr,reg,_pBuf,size);
  }
  return ret;
}