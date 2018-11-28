#include "ReadNoise.h"

NoiseRead::NoiseRead()
{

}


float NoiseRead::readNoise(void)
{
   i=0;
   Median=0;
   while (i<5)
    {
        RawRead[i]=analogRead(NOISE_MEASURE_PIN);
        delay(100);
	    i++;
    }
   for( i=0; i <5; i++)
   {
        k=i; x=RawRead[i];
        for( j=i+1; j < 5; j++)
        {
            if (  RawRead[j] < x )
            {
                k=j;
                x=RawRead[j];
            }
        }

        RawRead[k] = RawRead[i];
        RawRead[i] = x;
    }
    NoiseMedianVolts=float(RawRead[2])*0.0049-1.6;


	return(NoiseMedianVolts);
}


