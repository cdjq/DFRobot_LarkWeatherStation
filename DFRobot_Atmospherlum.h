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

class DFRobot_Atmospherlum : public DFRobot_RTU{

  #define CMD_READ_HOLDING             0x03
  #define CMD_READ_INPUT               0x04
  #define CMD_WRITE_MULTI_HOLDING      0x10
  #define DEVICE_ADDR                  0x02
  #define INPUT_WIND_SPEED             0x06
  #define INPUT_WIND_DIRECTION         0x07
  #define INPUT_TEMP                   0x08
  #define INPUT_HUMIDITY               0x09
  #define INPUT_PRESSURE_LOW           0x0A
  #define INPUT_ALTITUDE               0x0C
  #define INPUT_BATTERYVALUE           0x0D
  #define INPUT_BOARDSKU1              0x0E
  #define INPUT_BOARDSKU2              0x18

  #define HOLDING_STORAGETIME          0x07
  #define HOLDING_YEAR                 0x08
  #define HOLDING_MONTH                0x09
  #define HOLDING_DAY                  0x0A
  #define HOLDING_HOUR                 0x0B
  #define HOLDING_MINUTE               0x0C
  #define HOLDING_SECOND               0x0D
  #define HOLDING_WEEK                 0x0E
  #define HOLDING_POWER                0x0F
public:

  /**
   * @fn DFRobot_EnvironmentalSensor
   * @brief DFRobot_EnvironmentalSensor constructor
   * @param pWire I2C pointer to the TowWire stream, which requires calling begin in the demo to init Arduino I2C config.
   * @param addr  I2C communication address of SEN0500/SEN0501 device
   */
  DFRobot_Atmospherlum(uint8_t addr, TwoWire *pWire);

  /**
   * @fn DFRobot_EnvironmentalSensor
   * @brief DFRobot_EnvironmentalSensor constructor
   * @param addr: The device address of the communication between the host computer and SEN0500/SEN0501 slave device
   * @n     SEN0501_DEFAULT_DEVICE_ADDRESS or 32（0x20）: Default address of SEN0500/SEN0501 device, if users do not change the device address, it's default to 32.
   * @param s   : The serial port pointer to the Stream, which requires calling begin in the demo to init communication serial port config of Arduino main controller, in line with that of SEN0500/SEN0501 device slave.
   * @n SEN0500/SEN0501 serial port config: 9600 baud rate, 8-bit data bit, no check bit, 1 stop bit, the parameters can't be changed.
   */
  DFRobot_Atmospherlum(uint8_t addr, Stream *s);
  ~DFRobot_Atmospherlum(){};
  /**
   * @brief 初始化传感器
   * @return 返回初始化状态
   * @retval 0 通信成功
   * @retval 1 通信失败
   */
  uint8_t begin(void);
  /**
   * @brief 设置时间
   * @param time 需要设置的时间
   * @return NONE
   */
  void setTime(sTime_t time);
  
  String getValue(const char *str);
  String getUnit(const char *str);
  String getInformation(bool mode = false);
  String getTimeStamp();


protected:
  bool detectDeviceAddress(uint8_t addr);
  String convertHourMinuteSecond(sTime_t t);
  uint8_t  readReg(uint16_t reg, void *pBuf, uint8_t size,uint8_t stateReg);
  uint8_t writeReg(uint8_t reg, void *pBuf, size_t size);
  TwoWire   *_pWire = NULL;
  Stream    *_s = NULL;
  uint8_t   _addr;
  /**
   * @brief 后去RTC中时钟
   * @return 返回RTC中时间
   */
  sTime_t getTime(void);
  /**
   * @brief 获取云雀采集温度
   * @return 返回云雀采集温度
   */
  uint16_t getTemp(void);
  /**
   * @brief 获取云雀采集的湿度
   * @return 返回云雀采集的湿度
   */
  uint16_t getHUM(void);
  /**
   * @brief 获取云雀采集的风速
   * @return 返回云雀采集的风速
   */
  float getWindSpeed(void);
  /**
   * @brief 获取云雀采集的风向
   * @return 返回云雀采集的风向
   */
  String getWindDir(void);
  /**
   * @brief 获取云雀采集的气压
   * @return 返回云雀采集的气压
   */
  uint32_t getAirPressure(void);
  /**
   * @brief 获取云雀计算的海拔数据
   * @return 返回云雀计算的海拔数据
   */
  uint16_t getAltitude(void);
  /**
   * @brief 获取云雀板载电池电压
   * @return 返回云雀板载电池电压
   */
  uint16_t getBatteryCapacity(void);
  /**
   * @brief 获取云雀板载电池百分比
   * @return 返回云雀板载电池百分比
   */
  uint8_t getBattryPercentage(void);
  String getExtendData(void);
  String getDataTitel(uint16_t sku,void* buf);

};

#endif
