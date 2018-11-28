
#include <Wire.h>
#include "TSL2561.h"

TSL2561::TSL2561()
{
  
}

bool TSL2561::begin(void)
{
  Wire.begin();
  Wire.beginTransmission(TSL2561_ADDRESS);
  Wire.write(SELECT_CONTROL_REGISTER); // write 0x80 command to select the control register
  Wire.write(SET_POWER_UP_MODE);  // write 0x03 command to the control register to power up the sensor
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(TSL2561_ADDRESS);
  Wire.write(SELECT_TIMING_REGISTER); // write 0x81 command to select the timing register
  Wire.write(TIMING_SETTINGS);  // write 0x00 command to the timing register to set gain and integration time
  Wire.endTransmission();
}

unsigned int TSL2561::readVisAndIR(void)
{
	Wire.beginTransmission(TSL2561_ADDRESS);
	Wire.write(READ_CHANNEL_0); //Read two bytes from Channel 0 
	Wire.endTransmission();

	//Hang out while measurement is taken.
	delay(55);

	Wire.requestFrom(TSL2561_ADDRESS, 2);

	int counter = 0;
	while(Wire.available() < 2)
	{
		counter++;
		delay(1);
		if(counter > 100) return 998; //Error out
	}

	byte msb, lsb;
	msb = Wire.read();
	lsb = Wire.read();
	Wire.endTransmission();
	
	unsigned int IntensityCh0 = (unsigned int) msb + ((unsigned int) lsb)*256;

	return(IntensityCh0);
}

unsigned int TSL2561::readIRonly(void)
{
	Wire.beginTransmission(TSL2561_ADDRESS);
	Wire.write(READ_CHANNEL_1); //Read two bytes from Channel 0 
	Wire.endTransmission();

	//Hang out while measurement is taken.
	delay(55);

	Wire.requestFrom(TSL2561_ADDRESS, 2);

	int counter = 0;
	while(Wire.available() < 2)
	{
		counter++;
		delay(1);
		if(counter > 100) return 998; //Error out
	}

	byte msb, lsb;
	msb = Wire.read();
	lsb = Wire.read();
	Wire.endTransmission();
	
	unsigned int IntensityCh1 = (unsigned int) msb + ((unsigned int) lsb)*256;

	return(IntensityCh1);
}


