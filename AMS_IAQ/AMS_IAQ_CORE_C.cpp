#include "AMS_IAQ_CORE_C.h"

AMS_IAQ_CORE_C::AMS_IAQ_CORE_C() {

}

boolean AMS_IAQ_CORE_C::update(void){
    uint8_t bytes_received = 0;
    const long timeout_interval_ms = 100;
    unsigned long previousMillis = millis();
    unsigned long currentMillis = millis();
    uint8_t temp[9];
    
    Wire.requestFrom(AMS_IAQ_CORE_C_I2C_ADDRESS, 9, true); // START+SLA+R+STOP            
    while(Wire.available() && !((currentMillis - previousMillis) >= timeout_interval_ms)){
        currentMillis = millis();
        temp[bytes_received++] = Wire.read();      // DATA
        if(bytes_received == 9){
          break;
        }
    }
     
    if(bytes_received == 9){
      data[AMS_IAQ_CORE_C_STATUS_OFFSET] = temp[AMS_IAQ_CORE_C_STATUS_OFFSET];
      if(temp[AMS_IAQ_CORE_C_STATUS_OFFSET] == AMS_IAQ_CORE_C_STATUS_OK){
        for(uint8_t ii = 0; ii < 9; ii++){
          data[ii] = temp[ii];  
        }                     
        data[9] = 1;
      }       
      return true;
    }   
    
    return false;            
}

uint16_t AMS_IAQ_CORE_C::getCO2EquivalentPPM(void){
  uint16_t ret = data[AMS_IAQ_CORE_C_PREDICTION_MSB_OFFSET];
  ret <<= 8;
  ret += data[AMS_IAQ_CORE_C_PREDICTION_LSB_OFFSET];
  return ret;
}

uint32_t AMS_IAQ_CORE_C::getResistanceOhms(void){
  uint32_t ret = data[AMS_IAQ_CORE_C_RESISTANCE_MSB_OFFSET];
  ret <<= 8;
  ret += data[AMS_IAQ_CORE_C_RESISTANCE_MID_OFFSET];
  ret <<= 8;
  ret += data[AMS_IAQ_CORE_C_RESISTANCE_LSB_OFFSET];  
  return ret;    
}
  
uint16_t AMS_IAQ_CORE_C::getTVOCEquivalentPPB(void){
  uint16_t ret = data[AMS_IAQ_CORE_C_TVOC_EQUIVALENT_MSB_OFFSET];
  ret <<= 8;
  ret += data[AMS_IAQ_CORE_C_TVOC_EQUIVALENT_LSB_OFFSET];
  return ret;  
}
  
uint8_t AMS_IAQ_CORE_C::getStatus(void){
  switch(data[AMS_IAQ_CORE_C_STATUS_OFFSET]){
  case AMS_IAQ_CORE_C_STATUS_BUSY:
  case AMS_IAQ_CORE_C_STATUS_ERROR:
  case AMS_IAQ_CORE_C_STATUS_OK:
  case AMS_IAQ_CORE_C_STATUS_WARMING_UP:
    return data[AMS_IAQ_CORE_C_STATUS_OFFSET];
  default:
    return AMS_IAQ_CORE_C_STATUS_UNDEFINED;
  }
}
  