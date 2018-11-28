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

//double INOVA::readPM25conc(void) {
 INOVA::readPMconc(void) {   // ILKER
  j=0;
  i=0;


  for (i=0; i<10; i++)
  {
    data[i]=0;
  }
  //start[0]=0;
  i=0;
//  while (InovaPort.available() < 10 /* && i<1000 */)
//  {
    //  delay(1);
   //   i++;
//  }

//  Received =  InovaPort.available();

  InovaPort.readBytes(data,10);
  Received =  InovaPort.read();
   //checksum=(unsigned char)((int)data[2]+(int)data[3]+(int)data[4]+(int)data[5]+(int)data[6]+(int)data[7]);
	checksum=(data[2]+data[3]+data[4]+data[5]+data[6]+data[7]);
  MeasurementCount++;
  if (checksum == data[8])
  {
        PM25=(((double)data[3])*256+(double)data[2])/10;
        PM10=(((double)data[5])*256+(double)data[4])/10;
  }
  else WrongCheckSum++;
	#ifdef DEBUG
		Serial.print(data[0],HEX);Serial.print(".");Serial.println(data[1],HEX);
		Serial.print(data[2]);Serial.print(".");Serial.println(data[3]);
		Serial.print(data[4]);Serial.print(".");Serial.println(data[5]);
		Serial.print(data[6],HEX);Serial.print(".");Serial.println(data[7],HEX);
		Serial.print(data[8]);Serial.print(".");Serial.println(data[9],HEX);
		Serial.print(checksum);Serial.print(".");Serial.println(Received);

	#endif
}
/*
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
    //checksum=(int)data[1]+(int)data[2]+(int)data[3]+(int)data[4]-29;
	checksum=data[1]+data[2]+data[3]+data[4]+data[5]+data[6];					//cll,the checksum
    //PM25=(((double)data[2])*256+(double)data[1])/10;
 //   PM10=(((double)data[4])*256+(double)data[3])/10;
    //PM[0]=PM25;
    //PM[1]=PM10;
  }
  if (checksum==(int)data[7])
  {
			PM10=(((double)data[4])*256+(double)data[3])/10;
  }
  else
  {
  //    PM10=99.0;
      //PM[0]=99.0;
      //PM[1]=99.0;
  }
	return(PM10);
}
*/