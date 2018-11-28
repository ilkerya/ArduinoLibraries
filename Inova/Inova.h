
#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "SoftwareSerial.h"

#define DEBUG

class INOVA {
  public:
    INOVA(int rx, int tx);
    ~INOVA();
    bool begin(void);
    void listen(void);
    bool isListening(void);
//  double readPM25conc(void);
//  double readPM10conc(void);
  void readPMconc(void);
    float PM25;
    float PM10;
     unsigned char checksum;
    int PreambleNonRecog;
	int WrongCheckSum;
    int MeasurementCount;
  private:
    SoftwareSerial InovaPort;
    int i;
    int j;
 //   double PM25;
 //   double PM10;

    byte start[1];
    byte data[10];
    //double PM[2];
};


