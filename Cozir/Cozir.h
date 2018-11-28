
#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "SoftwareSerial.h"


class COZIR {
  public:
    COZIR(int rx, int tx);
    ~COZIR();
    bool begin(void);
    void listen(void);
    bool isListening(void);
  int readCO2conc(void);
  private:
    SoftwareSerial CozirPort;
    int i;
    int j;
    String ppmStr;
    int ppmInt;
    char portWide;
    char ppmWide[20];
};


