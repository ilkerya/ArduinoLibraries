#include "ReadVoc.h"

VocRead::VocRead()
{

}


float VocRead::readVOC(void)
{
   i=0;
   AverageRead=0;
   while (i<5)
    {
        RawRead=analogRead(VOC_MEASURE_PIN);
        delay(100);
        AverageRead=AverageRead+RawRead;
	    i++;
    }
   AverageRead=AverageRead/5;
   AverageRead=AverageRead*0.0049;

	return(AverageRead);
}


