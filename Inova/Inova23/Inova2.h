
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
 // double readPM25conc(void);
   readPMconc(void);
//  double readPM10conc(void);
    double PM25;
    double PM10;
    int PreambleNonRecog;
    int WrongCheckSum;
    int MeasurementCount;
    unsigned char checksum;
    unsigned char Received;
  private:
    SoftwareSerial InovaPort;
    int i;
    int j;
  //  double PM25;
  //  double PM10;
    //int checksum;

   // byte start[2];
    byte data[64];
    //double PM[2];
};


