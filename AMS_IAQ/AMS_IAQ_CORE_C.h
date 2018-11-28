#ifndef AMS_IAQ_CORE_C_H
#define AMS_IAQ_CORE_C_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Wire.h>

#define AMS_IAQ_CORE_C_I2C_ADDRESS          (0x5A)    /* Device Address */
#define AMS_IAQ_CORE_C_NOT_PRESENT          (0xA8)    // arbitrary library status code

#define AMS_IAQ_CORE_C_PREDICTION_MSB_OFFSET      (0x00)
#define AMS_IAQ_CORE_C_PREDICTION_LSB_OFFSET      (0x01)
#define AMS_IAQ_CORE_C_STATUS_OFFSET              (0x02)
#define AMS_IAQ_CORE_C_RESISTANCE_MSB_OFFSET      (0x04)
#define AMS_IAQ_CORE_C_RESISTANCE_MID_OFFSET      (0x05)
#define AMS_IAQ_CORE_C_RESISTANCE_LSB_OFFSET      (0x06)
#define AMS_IAQ_CORE_C_TVOC_EQUIVALENT_MSB_OFFSET (0x07)
#define AMS_IAQ_CORE_C_TVOC_EQUIVALENT_LSB_OFFSET (0x08)

#define AMS_IAQ_CORE_C_STATUS_OK                  (0x00)
#define AMS_IAQ_CORE_C_STATUS_BUSY                (0x01)
#define AMS_IAQ_CORE_C_STATUS_ERROR               (0x80)
#define AMS_IAQ_CORE_C_STATUS_WARMING_UP          (0x10)
#define AMS_IAQ_CORE_C_STATUS_UNDEFINED           (0xFF)

class AMS_IAQ_CORE_C {

public:
  AMS_IAQ_CORE_C();
  boolean update(void);
  uint8_t data[10]; // data[9] set to 1 when at least one valid reading exists  
  
  uint16_t getCO2EquivalentPPM(void);
  uint32_t getResistanceOhms(void);  
  uint16_t getTVOCEquivalentPPB(void);   
  uint8_t getStatus(void);

private:
};

#endif
