
#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>


// ILKER
#include <Selection.h>


#define SELECT_CONTROL_REGISTER  0x80
#define SET_POWER_UP_MODE  0x03
#define SELECT_TIMING_REGISTER  0x81
#define TIMING_SETTINGS  0x00  //gain is 1, default inegration time 13.7ms
#define READ_CHANNEL_0 0xAC  // command to read two bytes from channel 0
#define READ_CHANNEL_1 0xAE  // command to read two bytes from channel 1

class TSL2561 {

public:
  TSL2561();

  //Public Functions
  bool begin();
  unsigned int readVisAndIR(void);
  unsigned int readIRonly(void);

  //Public Variables

private:
  //Private Functions

  //Private Variables

};
