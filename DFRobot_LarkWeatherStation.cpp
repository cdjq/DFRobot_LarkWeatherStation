/*!
 * @file  DFRobot_LarkWeatherStation.cpp
 * @brief DFRobot_LarkWeatherStation Class infrastructure, implementation of basic methods
 *
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0
 * @date      2021-08-31
 * @url       https://github.com/DFRobot/DFRobot_LarkWeatherStation
 */
#include "DFRobot_LarkWeatherStation.h"

#define DEBUG_TIMEOUT_MS    4500

#define CMD_GET_DATA                0x00 // Return name based on the passed name
#define CMD_GET_ALL_DATA            0x01 // Get all onboard sensor data
#define CMD_SET_TIME                0x02 // Set onboard RTC time
#define CME_GET_TIME                0x03
#define CMD_GET_UNIT                0x04 // Get sensor units
#define CMD_GET_VERSION             0x05 // Get version number
#define CMD_RESET_DATA              0x06 // Re-transmit data
#define CMD_RADIUS_DATA             0x07 // Set cup radius
#define CMD_SPEED1_DATA             0x08 // Set standard wind speed 1
#define CMD_SPEED2_DATA             0x09 // Set standard wind speed 2
#define CMD_CALIBRATOR              0x0a // Start calibration calculation

#define CMD_DTU                     0x0c // DTU configuration
#define CMD_WIFI                    0x0d // WiFi configuration
#define CMD_LORA                    0x0e // LoRa configuration
#define CMD_MQTT1                   0x10 // MQTT configuration
#define CMD_MQTT2                   0x11 // MQTT configuration
#define CMD_TOP                     0x12 // TOPIC configuration


#define IIC_MAX_TRANSFER            32     ///< Maximum transferred data via I2C
#define I2C_ACHE_MAX_LEN            32
#define CMD_END             CMD_TOP


#define ERR_CODE_NONE               0x00 ///< Normal communication 
#define ERR_CODE_CMD_INVAILED       0x01 ///< Invalid command
#define ERR_CODE_RES_PKT            0x02 ///< Response packet error
#define ERR_CODE_M_NO_SPACE         0x03 ///< Insufficient memory of I2C controller(master)
#define ERR_CODE_RES_TIMEOUT        0x04 ///< Response packet reception timeout
#define ERR_CODE_CMD_PKT            0x05 ///< Invalid command packet or unmatched command
#define ERR_CODE_SLAVE_BREAK        0x06 ///< Peripheral(slave) fault
#define ERR_CODE_ARGS               0x07 ///< Set wrong parameter
#define ERR_CODE_SKU                0x08 ///< The SKU is an invalid SKU, or unsupported by SCI Acquisition Module
#define ERR_CODE_S_NO_SPACE         0x09 ///< Insufficient memory of I2C peripheral(slave)
#define ERR_CODE_I2C_ADRESS         0x0A ///< Invalid I2C address

#define STATUS_SUCCESS      0x53  ///< Status of successful response   
#define STATUS_FAILED       0x63  ///< Status of failed response 

typedef struct{
  uint8_t cmd;      /**< Command                     */
  uint8_t argsNumL; /**< Low byte of parameter number after the command    */
  uint8_t argsNumH; /**< High byte of parameter number after the command    */
  uint8_t args[0];  /**< The array with 0-data length, its size depends on the value of the previous variables argsNumL and argsNumH     */
}__attribute__ ((packed)) sCmdSendPkt_t, *pCmdSendPkt_t;

typedef struct{
  uint8_t status;   /**< Response packet status, 0x53, response succeeded, 0x63, response failed */
  uint8_t cmd;      /**< Response packet command */
  uint8_t lenL;     /**< Low byte of the buf array length excluding packet header */
  uint8_t lenH;     /**< High byte of the buf array length excluding packet header */
  uint8_t buf[0];   /**< The array with 0-data length, its size depends on the value of the previous variables lenL and lenH */
}__attribute__ ((packed)) sCmdRecvPkt_t, *pCmdRecvPkt_t;


String DFRobot_LarkWeatherStation::getValue(char *keys)
{
  String values = "";
  uint8_t errorCode;
  uint16_t length = 0;
  if(keys == NULL) return values;
  length = strlen(keys);

  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_GET_DATA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, keys, strlen(keys));
  
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_GET_DATA, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    char sku[length + 1];
    memcpy(sku, rcvpkt->buf, length);
    sku[length] = '\0';
    values = String(sku);
  }
  if(rcvpkt) free(rcvpkt);
  return values;

}
String DFRobot_LarkWeatherStation::getUnit(char *keys)
{
  String values = "";
  uint8_t errorCode;
  uint16_t length = 0;
  if(keys == NULL) return values;
  length = strlen(keys);

  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_GET_UNIT;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, keys, strlen(keys));
  
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_GET_UNIT, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    char sku[length + 1];
    memcpy(sku, rcvpkt->buf, length);
    sku[length] = '\0';
    values = String(sku);
  }
  if(rcvpkt) free(rcvpkt);
  return values;
}

 int DFRobot_LarkWeatherStation::setRadius(float radius){
  uint16_t data = radius * 100;
  uint16_t length = 2;
  uint8_t errorCode;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return 0;
  sendpkt->cmd = CMD_RADIUS_DATA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  sendpkt->args[0] = data >> 8;
  sendpkt->args[1] = data & 0xff;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(2000);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_RADIUS_DATA, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    if(rcvpkt) free(rcvpkt);
    DBG("setRadius");
    return 1;
  }
  return 0;
 }

 void DFRobot_LarkWeatherStation::projectMode(void){
  uint16_t length = 0;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  //if(sendpkt == NULL) return "";
  sendpkt->cmd = 0x0b;
  sendpkt->argsNumL = 0;
  sendpkt->argsNumH = 0;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
 }

 void DFRobot_LarkWeatherStation::setSpeed1(float speed){
   uint16_t data = speed * 100;
  uint16_t length = 2;
  uint8_t errorCode;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return 0;
  sendpkt->cmd = CMD_SPEED1_DATA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  sendpkt->args[0] = data >> 8;
  sendpkt->args[1] = data & 0xff;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(10000);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_SPEED1_DATA, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    if(rcvpkt) free(rcvpkt);
  }
 }
 void DFRobot_LarkWeatherStation::setSpeed2(float speed){
   uint16_t data = speed * 100;
  uint16_t length = 2;
  uint8_t errorCode;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return 0;
  sendpkt->cmd = CMD_SPEED2_DATA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  sendpkt->args[0] = data >> 8;
  sendpkt->args[1] = data & 0xff;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(10000);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_SPEED2_DATA, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    if(rcvpkt) free(rcvpkt);
  }
 }
 String DFRobot_LarkWeatherStation::calibrationSpeed(void){
  String values = "";
  uint16_t length = 0;
  uint8_t errorCode;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_CALIBRATOR;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(2000);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_CALIBRATOR, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    char sku[length + 1];
    memcpy(sku, rcvpkt->buf, length);
    sku[length] = '\0';
    values = String(sku);
  }
  if(rcvpkt) free(rcvpkt);
  return values;
 }

String DFRobot_LarkWeatherStation::getInformation(bool state)
{
  String values = "";
  uint8_t errorCode;
  uint16_t length = 1;

  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_GET_ALL_DATA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  if(state == true){
    sendpkt->args[0] = 1;
  }else{
    sendpkt->args[0] = 0;
  }
    
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  DBG("send ok");
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_GET_ALL_DATA, &errorCode);
  DBG("return");
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)/*|| (rcvpkt->status == 0xD3)*/){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    char sku[length + 1];
    memcpy(sku, rcvpkt->buf, length);
    sku[length] = '\0';
    values = String(sku);
  }else{
    DBG("rcvpkt->status");
    DBG(rcvpkt->status);
  }
  if(rcvpkt) free(rcvpkt);
  return values;
}

void * DFRobot_LarkWeatherStation::recvPacket(uint8_t cmd, uint8_t *errorCode){
  if(cmd > CMD_END){
    DBG("cmd is error!");
    if(errorCode) *errorCode = ERR_CODE_CMD_INVAILED; //There is no this command
    return NULL;
  }
  
  sCmdRecvPkt_t recvPkt;
  pCmdRecvPkt_t recvPktPtr = NULL;
  uint16_t length = 0;
  uint32_t t = millis();
  while(millis() - t < _timeout/*time_ms*/){
    DBG("k");
    recvData(&recvPkt.status, 1);
    if(recvPkt.status != 0xff){
      switch(recvPkt.status){
        case STATUS_SUCCESS:
        case STATUS_FAILED:{
          recvData(&recvPkt.cmd, 1);
          if(recvPkt.cmd != cmd){
            //recvFlush();
            if(errorCode) *errorCode = ERR_CODE_RES_PKT; //Response packet error
            DBG("Response pkt is error!");
            return NULL;
          }
          recvData(&recvPkt.lenL, 2);
          length = (recvPkt.lenH << 8) | recvPkt.lenL;
          DBG(length);
          if(length<1000){
            recvPktPtr = (pCmdRecvPkt_t)malloc(sizeof(sCmdRecvPkt_t) + length);
          }else{
             return NULL;
           }
          
          if(recvPktPtr == NULL){
            if(errorCode) *errorCode = ERR_CODE_M_NO_SPACE; //Insufficient memory of I2C controller(master)
            DBG("malloc error");
            free(recvPktPtr);
            return NULL;
          }
          memcpy(recvPktPtr, &recvPkt, sizeof(sCmdRecvPkt_t));
      
          if(length)recvData(recvPktPtr->buf, length);
          if(errorCode) *errorCode = ERR_CODE_NONE;
          return recvPktPtr;
      }
      default:
        restData();
        delay(50);
        break;
    }
    }
    
    delay(50);
    //yield();
  }
  if(errorCode) *errorCode = ERR_CODE_RES_TIMEOUT; //Receive packet timeout
  free(recvPktPtr);
  DBG("Time out!");
  DBG(millis() - t);
  return NULL;
}

void DFRobot_LarkWeatherStation::restData(void){
  uint16_t length = 0;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  //if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_RESET_DATA;
  sendpkt->argsNumL = 0;
  sendpkt->argsNumH = 0;
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
}

uint8_t DFRobot_LarkWeatherStation::setTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second){
  uint16_t length = 7;
  uint8_t errorCode;
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_SET_TIME;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  sendpkt->args[0] = (year-2000);
  sendpkt->args[1] = month;
  sendpkt->args[2] = day;
  sendpkt->args[3] = 0;
  sendpkt->args[4] = hour;
  sendpkt->args[5] = minute;
  sendpkt->args[6] = second;
    
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_SET_TIME, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("set time");
    return 1;
  }
  return 0;
  
}

String DFRobot_LarkWeatherStation::getTimeStamp(){
  String values = "";
  uint8_t errorCode;
  uint16_t length = 0;

  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CME_GET_TIME;
  sendpkt->argsNumL = 0;
  sendpkt->argsNumH = 0;
 
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CME_GET_TIME, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    char sku[length + 1];
    memcpy(sku, rcvpkt->buf, length);
    sku[length] = '\0';
    values = String(sku);
  }
  if(rcvpkt) free(rcvpkt);
  return values;
}

DFRobot_LarkWeatherStation::DFRobot_LarkWeatherStation()
  :_timeout(DEBUG_TIMEOUT_MS){}

DFRobot_LarkWeatherStation::~DFRobot_LarkWeatherStation(){}

int DFRobot_LarkWeatherStation::begin(uint32_t freq){
  return init(freq);
}
DFRobot_LarkWeatherStation_I2C::DFRobot_LarkWeatherStation_I2C(uint8_t addr, TwoWire *pWire)
  :DFRobot_LarkWeatherStation(),_pWire(pWire),_addr(addr){
  
}

DFRobot_LarkWeatherStation_I2C::~DFRobot_LarkWeatherStation_I2C(){}

int DFRobot_LarkWeatherStation_I2C::init(uint32_t freq){
  if (_pWire == NULL) return -1;
  _pWire->begin();
  _pWire->setClock(freq);
  _pWire->beginTransmission(_addr);
  if(_pWire->endTransmission() != 0) return -2;
  return 0;
}

void DFRobot_LarkWeatherStation_I2C::sendPacket(void *pkt, int length, bool stop){
  uint8_t *pBuf = (uint8_t *)pkt;
  int remain = length;
  if((pkt == NULL) || (length == 0)) return;
  _pWire->beginTransmission(_addr);
  while(remain){
     //DBG("a");
    length = (remain > IIC_MAX_TRANSFER) ? IIC_MAX_TRANSFER : remain;
    _pWire->write(pBuf, length);
    remain -= length;
    pBuf += length;
#if defined(ESP32)
    if(remain) _pWire->endTransmission(true);
#else
    if(remain) _pWire->endTransmission(false);
#endif
  }
  _pWire->endTransmission();
}

int DFRobot_LarkWeatherStation_I2C::recvData(void *data, int len){
  uint8_t *pBuf = (uint8_t *)data;
  int remain = len;
  int total = 0;
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  //DBG(len);
  while(remain){
     //DBG("c");
    len = remain > I2C_ACHE_MAX_LEN ? I2C_ACHE_MAX_LEN : remain;
    remain -= len;
#if defined(ESP32)
    if(remain) {_pWire->requestFrom(_addr, len, true);}
#else
    if(remain){ _pWire->requestFrom(_addr, len, false);}
#endif
    else{_pWire->requestFrom(_addr, len, true);}
    for(int i = 0; i < len; i++){
      pBuf[i] = _pWire->read();
      //DBG(pBuf[i],HEX);
      //delay(1);
      yield();
    }
    pBuf += len;
    total += len;
  }
  return total;
}

void DFRobot_LarkWeatherStation_I2C::recvFlush(){
  while(_pWire->available()){
     //DBG("d");
    _pWire->read();
    yield();
  }
}

void DFRobot_LarkWeatherStation_I2C::sendFlush(){
  _pWire->flush();
}

DFRobot_LarkWeatherStation_UART::DFRobot_LarkWeatherStation_UART(Stream *s)
:DFRobot_LarkWeatherStation()
{
  _s = s;
}
DFRobot_LarkWeatherStation_UART::~DFRobot_LarkWeatherStation_UART(){}

int DFRobot_LarkWeatherStation_UART::init(uint32_t freq){
  //_s->begin(115200);
  return 0;
}

void DFRobot_LarkWeatherStation_UART::sendPacket(void *pkt, int length, bool stop){
  uint8_t *pBuf = (uint8_t *)pkt;
  int remain = length;
  if((pkt == NULL) || (length == 0)) return;
  for(uint8_t i = 0; i < remain; i++){
    _s->write(pBuf[i]);
    delay(1);
  }
   
}

int DFRobot_LarkWeatherStation_UART::recvData(void *data, int len)
{
  // uint8_t *pBuf = (uint8_t *)data;
  // int remain = len;
  // int total = 0;
  // uint32_t firstTime = 0;
  // while(1){
  //     if(_s->available()!= 0){
  //       firstTime = millis();
  //       pBuf[total] = _s->read();
  //       total++;
  //       if(remain == total){
  //         // for(int i = 0; i < len; i++){
  //         //   Serial.print(pBuf[i]);
  //         // }
  //         return total;
  //       }
  //     }
  //   if((millis() - firstTime) > 30000){
  //     DBG("serial time out");
  //     return total;
  //   }
  //   delay(1);
  // }

  uint8_t *pBuf = (uint8_t *)data;
  int remain = len;
  int total = 0;
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  
  while(remain){
    len = remain > I2C_ACHE_MAX_LEN ? I2C_ACHE_MAX_LEN : remain;
    remain -= len;
    _s->readBytes(pBuf, len);
    pBuf += len;
    total += len;
  }
  return total;
}

void DFRobot_LarkWeatherStation_UART::recvFlush()
{
   while(_s->available()){
    _s->read();
    delay(1);
  }
}
void DFRobot_LarkWeatherStation_UART::sendFlush(){
  
}

uint8_t DFRobot_LarkWeatherStation::configDTU(char* dtuswitch, char* method){
  uint16_t length;
  uint8_t errorCode;
  String str = String(dtuswitch) +"," + String(method);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_DTU;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_DTU, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configDTU");
    return 1;
  }
  return 0;
}

uint8_t DFRobot_LarkWeatherStation::configWIFI(char* SSID, char* PWD){
  uint16_t length;
  uint8_t errorCode;
  String str = String(SSID) +"," + String(PWD);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_WIFI;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_WIFI, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configWIFI");
    return 1;
  }
  return 0;
} 
uint8_t DFRobot_LarkWeatherStation::configLora(char* DEUI, char* EUI,char* KEY){
  uint16_t length;
  uint8_t errorCode;
  String str = String(DEUI) +"," + String(EUI) + "," + String(KEY);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_LORA;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_LORA, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configLora");
    return 1;
  }
  return 0;
}

uint8_t DFRobot_LarkWeatherStation::configMQTT1(char* Server, char* Server_IP,char* Save){
  uint16_t length;
  uint8_t errorCode;
  String str = String(Server) +"," + String(Server_IP) + "," + String(Save);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_MQTT1;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_MQTT1, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configMQTT1");
    return 1;
  }
  return 0;
}

uint8_t DFRobot_LarkWeatherStation::configMQTT2(char* Iot_ID,char* Iot_PWD){
  uint16_t length;
  uint8_t errorCode;
  String str = String(Iot_PWD) + "," + String(Iot_ID);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_MQTT2;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_MQTT2, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configMQTT2");
    return 1;
  }
  return 0;
}

uint8_t DFRobot_LarkWeatherStation::configTopic(char* name,char* chan){
  uint16_t length;
  uint8_t errorCode;
  String str = String(name) + ":" + String(chan);
  length = strlen(str.c_str());
  pCmdSendPkt_t sendpkt = NULL;
  sendpkt = (pCmdSendPkt_t)malloc(sizeof(sCmdSendPkt_t) + length);
  if(sendpkt == NULL) return "";
  sendpkt->cmd = CMD_TOP;
  sendpkt->argsNumL = length & 0xFF;
  sendpkt->argsNumH = (length >> 8) & 0xFF;
  memcpy(sendpkt->args, str.c_str(), strlen(str.c_str()));
  length += sizeof(sCmdSendPkt_t);
  
  sendPacket(sendpkt, length, true);
  free(sendpkt);
  delay(100);
  pCmdRecvPkt_t rcvpkt = (pCmdRecvPkt_t)recvPacket(CMD_TOP, &errorCode);
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_FAILED)) errorCode = rcvpkt->buf[0];
  if((rcvpkt != NULL) && (rcvpkt->status == STATUS_SUCCESS)){
    length = (rcvpkt->lenH << 8) | rcvpkt->lenL;
    if(rcvpkt) free(rcvpkt);
    DBG("configTopic");
    return 1;
  }
  return 0;
}