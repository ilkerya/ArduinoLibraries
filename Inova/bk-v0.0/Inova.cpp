#include "Inova.h"

INOVA::INOVA(int rx, int tx):InovaPort(rx, tx) {
  
}
INOVA::~INOVA() {}

bool INOVA::begin(void)
{
    InovaPort.begin(9600);
}
void INOVA::listen(void)
{
    InovaPort.listen();
}
bool INOVA::isListening(void)
{
    InovaPort.isListening();
}

double INOVA::readPM25conc(void) {
  j=0;
  i=0;
  
  
  for (i=0; i<10; i++)
  {
    data[i]=0;
  }
  start[0]=0;
  i=0;
  while (InovaPort.available()<=0 && i<1000)
  {
      delay(1);
      i++;
  }
  if (InovaPort.available())
  {
    InovaPort.readBytes(start,1);
    while (start[0]!=0xAA)
    {
      InovaPort.readBytes(start,1);
      delay(1);
      
    }
    
    InovaPort.readBytes(data,9);
    checksum=(int)data[1]+(int)data[2]+(int)data[3]+(int)data[4]-29;
    //PM25=(int)data[1];
    PM25=(((double)data[2])*256+(double)data[1])/10;
    //PM10=(((double)data[4])*256+(double)data[3])/10;
    //PM[0]=PM25;
    //PM[1]=PM10;
  }
  
  /*if (checksum==(int)data[7])
  {
      
  }
  else
  {
      PM25=99.0;
      //PM[0]=99.0;
      //PM[1]=99.0; 
  }*/
	return(PM25);
}
double INOVA::readPM10conc(void) {
  j=0;
  i=0;
  
  
  for (i=0; i<10; i++)
  {
    data[i]=0;
  }
  start[0]=0;
  i=0;
  while (InovaPort.available()<=0 && i<1000)
  {
      delay(1);
      i++;
  }
  if (InovaPort.available())
  {
    InovaPort.readBytes(start,1);
    while (start[0]!=0xAA)
    {
      InovaPort.readBytes(start,1);
      delay(1);
      
    }
    
    InovaPort.readBytes(data,9);
    checksum=(int)data[1]+(int)data[2]+(int)data[3]+(int)data[4]-29;
    //PM25=(((double)data[2])*256+(double)data[1])/10;
    PM10=(((double)data[4])*256+(double)data[3])/10;
    //PM[0]=PM25;
    //PM[1]=PM10;
  }
  /*if (checksum==(int)data[7])
  {
      
  }
  else
  {
      PM10=99.0;
      //PM[0]=99.0;
      //PM[1]=99.0; 
  }*/
	return(PM10);
}