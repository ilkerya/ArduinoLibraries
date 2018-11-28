#include "Cozir.h"

COZIR::COZIR(int rx, int tx):CozirPort(rx, tx) {
  
}
COZIR::~COZIR() {}

bool COZIR::begin(void)
{
    CozirPort.begin(9600);
}
void COZIR::listen(void)
{
    CozirPort.listen();
}
bool COZIR::isListening(void)
{
    CozirPort.isListening();
}

int COZIR::readCO2conc(void) {
  j=0;
  i=0;
  ppmStr="";
  ppmInt=0;
  CozirPort.write("Z\r\n");
  delay(10);
  while (CozirPort.available()<=0 && i<1000)
  {
      delay(1);
      i++;
  }
  i=0;
  //CozirPort.write("Z\r\n");
  delay(1);
  if (CozirPort.available())
  {
    portWide=CozirPort.read();
    while (portWide!=0x0A)
    {
      portWide=CozirPort.read();
      delay(1);
      ppmWide[i]=portWide;
      j=i;
      i++;
    }
    
  }
  for (i=0; i<=j-1; i++)
  {
    
      ppmStr+=ppmWide[i];
  }
  ppmStr=ppmStr.substring(2);
  ppmInt=ppmStr.toInt();

	return(ppmInt);
}