#ifndef _DFROBOT_ATMOSPHERLUM_H_
#define _DFROBOT_ATMOSPHERLUM_H_

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_RTU.h"
#include "String.h"

#if (defined ARDUINO_AVR_UNO) && (defined ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

#define ENABLE_DBG ///< 打开这个宏, 可以看到程序的详细运行过程
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

typedef struct{
    uint16_t year;
    uint16_t  month;
    uint16_t  day;
    uint16_t  hour;
    uint16_t  minute;
    uint16_t  second;
    uint16_t  week;
  }sTime_t;


class DFRobot_Atmospherlum{
public:

  /**
   * @fn DFRobot_RP2040_SCI
   * @brief DFRobot_RP2040_SCI Class Constructor.
   */
  DFRobot_Atmospherlum();

  /**
   * @fn  ~DFRobot_RP2040_SCI
   * @brief DFRobot_RP2040_SCI Class Destructor. 
   */
  ~DFRobot_Atmospherlum();

  /**
   * @fn begin
   * @brief Initalize the SCI Acquisition Module, mainly for initializing communication interface
   * 
   * @param freq Set communication frequency, no more than 100kHz
   * @return int Init status
   * @n       0  Init successful
   * @n      -1  The communication interface class & object are not passed in
   * @n      -2  Check if the hardware connection is correct
   */
  int begin(uint32_t freq = 100000);

  String getValue(char *keys);
  String getUnit(char *keys);
  String getInformation(bool state);
  void setTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
  String getTimeStamp(void);

protected:
  // uint32_t getRefreshRate_ms(uint8_t rate);
  // /**
  //  * @fn recvPacket
  //  * @brief Receive and parse the response data packet
  //  * 
  //  * @param cmd       Command to receive packet
  //  * @param errorCode Receive error code
  //  * @return Pointer array
  //  * @n      NULL    indicates receiving packet failed
  //  * @n      Non-NULL  response packet pointer
  //  */
  void *recvPacket(uint8_t cmd, uint8_t *errorCode);
  /**
   * @fn init
   * @brief Pure virtual function, interface init
   * 
   * @param freq     Communication frequency
   * @return Init status
   * @n       0    Init succeeded
   * @n      -1    Interface object is null pointer
   * @n      -2    Device does not exist
   */
  virtual int init(uint32_t freq) = 0;
  /**
   * @fn sendPacket
   * @brief I2C interface init
   * 
   * @param pkt    Set I2C communication frequency
   * @param length Set I2C communication frequency
   * @param stop   
   * @n     true   Stop
   * @n     false  Not stop
   */
  virtual void sendPacket(void *pkt, int length, bool stop) = 0;
  /**
   * @fn recvData
   * @brief I2C interface init
   * 
   * @param data    Store the received data cache
   * @param len     Byte number to be read
   * @return Actually read byte number    
   */
  virtual int recvData(void *data, int len) = 0;
  /**
   * @fn recvFlush
   * @brief Clear receive cache
   */
  virtual void recvFlush() = 0;
  /**
   * @fn sendFlush
   * @brief Clear send cache
   */
  virtual void sendFlush() = 0;

private:
  uint32_t _timeout; ///< Time of receive timeout
};

class DFRobot_Atmospherlum_I2C:public DFRobot_Atmospherlum {


public:

DFRobot_Atmospherlum_I2C(uint8_t addr = 0x42, TwoWire *pWire = &Wire);
~DFRobot_Atmospherlum_I2C();
  protected:
  /**
   * @fn init
   * @brief Initalize I2C interface
   * 
   * @param freq Set I2C communication frequency
   * @return int Init status
   * @n       0  Init successful
   * @n      -1  The communication interface class & object are not passed in
   * @n      -2  Check if the hardware connection is correct
   */
  int init(uint32_t freq);
 /**
   * @fn sendPacket
   * @brief Send data
   * 
   * @param pkt    Data pointer
   * @param length Length of the data to be sent
   * @param stop   Whether to send stop condition
   * @n     true   Stop
   * @n     false  Not stop
   */
  void sendPacket(void *pkt, int length, bool stop = true);
  /**
   * @fn recvData
   * @brief I2C read byte
   * 
   * @param data    Received and stored data cache
   * @param len     Byte number to be read
   * @return Actually read byte number   
   */
  int recvData(void *data, int len);
  /**
   * @fn recvFlush
   * @brief Clear receive cache
   */
  void recvFlush();
  /**
   * @fn sendFlush
   * @brief Clear send cache
   */
  void sendFlush();
private:
  TwoWire *_pWire;
  uint8_t _addr;
};

class DFRobot_Atmospherlum_UART:public DFRobot_Atmospherlum {

public:

DFRobot_Atmospherlum_UART(Stream *s);
~DFRobot_Atmospherlum_UART();
protected:
   /**
   * @fn init
   * @brief Initalize I2C interface
   * 
   * @param freq Set I2C communication frequency
   * @return int Init status
   * @n       0  Init successful
   * @n      -1  The communication interface class & object are not passed in
   * @n      -2  Check if the hardware connection is correct
   */
  int init(uint32_t freq);
 /**
   * @fn sendPacket
   * @brief Send data
   * 
   * @param pkt    Data pointer
   * @param length Length of the data to be sent
   * @param stop   Whether to send stop condition
   * @n     true   Stop
   * @n     false  Not stop
   */
  void sendPacket(void *pkt, int length, bool stop = true);
  /**
   * @fn recvData
   * @brief I2C read byte
   * 
   * @param data    Received and stored data cache
   * @param len     Byte number to be read
   * @return Actually read byte number   
   */
  int recvData(void *data, int len);
  /**
   * @fn recvFlush
   * @brief Clear receive cache
   */
  void recvFlush();
  /**
   * @fn sendFlush
   * @brief Clear send cache
   */
  void sendFlush();
private:
  uint8_t state = 0;
  Stream *_s;
};

#endif
