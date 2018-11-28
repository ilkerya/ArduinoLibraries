
#include <Wire.h>
#include "SIm7013.h"

SI7013::SI7013()
{
  
}

bool SI7013::begin(void)
{
  Wire.begin();
}

float SI7013::readHumidity(void)
{
	//Request a humidity reading
	Wire.beginTransmission(SI7013_ADDRESS);
	Wire.write(MEASURE_RH_NOHOLD); //Measure humidity with no bus holding
	Wire.endTransmission();

	//Hang out while measurement is taken.
	delay(55);

	//Comes back in three bytes, data(MSB) / data(LSB) / Checksum
	Wire.requestFrom(SI7013_ADDRESS, 3);

	//Wait for data to become available
	int counter = 0;
	while(Wire.available() < 3)
	{
		counter++;
		delay(1);
		if(counter > 100) return 998; //Error out
	}

	byte msb, lsb, checksum;
	msb = Wire.read();
	lsb = Wire.read();
	checksum = Wire.read();

	unsigned int rawHumidity = ((unsigned int) msb << 8) | (unsigned int) lsb;

	if(check_crc(rawHumidity, checksum) != 0) return(999); //Error out

	//sensorStatus = rawHumidity & 0x0003; //Grab only the right two bits
	rawHumidity &= 0xFFFC; //Zero out the status bits but keep them in place
	
	//Given the raw humidity data, calculate the actual relative humidity
	float tempRH = rawHumidity / (float)65536; //2^16 = 65536
	float rh = -6 + (125 * tempRH); //From page 14
	
	return(rh);
}

//Read the temperature
float SI7013::readTemperature(void)
{
	//Request the temperature
	Wire.beginTransmission(SI7013_ADDRESS);
	Wire.write(MEASURE_TEMP_NOHOLD);
	Wire.endTransmission();

	//Hang out while measurement is taken.
	delay(55);

	//Comes back in three bytes, data(MSB) / data(LSB) / Checksum
	Wire.requestFrom(SI7013_ADDRESS, 3);

	//Wait for data to become available
	int counter = 0;
	while(Wire.available() < 3)
	{
		counter++;
		delay(1);
		if(counter > 100) return 998; //Error out
	}

	unsigned char msb, lsb, checksum;
	msb = Wire.read();
	lsb = Wire.read();
	checksum = Wire.read();

	/* //Used for testing
	byte msb, lsb, checksum;
	msb = 0x68;
	lsb = 0x3A;
	checksum = 0x7C; */

	unsigned int rawTemperature = ((unsigned int) msb << 8) | (unsigned int) lsb;

	if(check_crc(rawTemperature, checksum) != 0) return(999); //Error out

	//sensorStatus = rawTemperature & 0x0003; //Grab only the right two bits
	rawTemperature &= 0xFFFC; //Zero out the status bits but keep them in place

	//Given the raw temperature data, calculate the actual temperature
	float tempTemperature = rawTemperature / (float)65536; //2^16 = 65536
	float realTemperature = -46.85 + (175.72 * tempTemperature); //From page 14

	return(realTemperature);  
}

unsigned int SI7013::read_user_register_heater(void)
{
  unsigned int userRegisterHeater;
  
  //Request the user register 3
  Wire.beginTransmission(SI7013_ADDRESS);
  Wire.write(READ_USER_REG3); //Read the user register
  Wire.endTransmission();
  
  delay(55);
  //Read result
  Wire.requestFrom(SI7013_ADDRESS, 1);
  
  int counter = 0;
	while(Wire.available() < 1)
	{
		counter++;
		delay(1);
		if(counter > 100) return 998; //Error out
	}

	byte msbHeat; //, lsbmy; // checksum;
	msbHeat = Wire.read();
	//lsbmy = Wire.read();
	
	userRegisterHeater = (unsigned int) msbHeat; // << 1; //) | (unsigned int) lsb;
    
    

  return(userRegisterHeater);  
}


void SI7013::heater_enable(void)
{
  byte userReg1=read_user_register();
  byte heaterEnable=0x4;
  byte toWriteReg1Enable=userReg1 | heaterEnable;
  
  Wire.beginTransmission(SI7013_ADDRESS);
  Wire.write(WRITE_USER_REG1); //Write to the user register
  Wire.write(toWriteReg1Enable); //Write the new bit for enabling
  Wire.endTransmission();
  
  
  
}
void SI7013::heater_disable(void)
{
  byte userReg1=read_user_register();
  byte heaterDisable=0x3A;
  byte toWriteReg1Disable=userReg1 & heaterDisable;
  
  Wire.beginTransmission(SI7013_ADDRESS);
  Wire.write(WRITE_USER_REG1); //Write to the user register
  Wire.write(toWriteReg1Disable); //Write the new bit for disabling
  Wire.endTransmission();
  
}
void SI7013::heater_current(byte SetCurrent)
{
  byte userReg3=read_user_register_heater();
  //byte heaterCurrent15=0x4;
  byte heaterCurrent=SetCurrent;
  byte toWriteReg3Current=userReg3 | heaterCurrent;
  
  Wire.beginTransmission(SI7013_ADDRESS);
  Wire.write(WRITE_USER_REG3); //Write to the user register
  Wire.write(toWriteReg3Current); //Write the new bit for 15mA
  Wire.endTransmission();
  
}
void SI7013::heater_current_default(void)
{
  byte userReg3=read_user_register_heater();
  byte heaterCurrentDefault=0x00;
  byte toWriteReg3Current=userReg3 & heaterCurrentDefault;
  
  Wire.beginTransmission(SI7013_ADDRESS);
  Wire.write(WRITE_USER_REG3); //Write to the user register
  Wire.write(toWriteReg3Current); //Write the new bit for 15mA
  Wire.endTransmission();
  
}

#define SHIFTED_DIVISOR 0x988000 //This is the 0x0131 polynomial shifted to farthest left of three bytes

byte SI7013::check_crc(uint16_t message_from_sensor, uint8_t check_value_from_sensor)
{
  //Test cases from datasheet:
  //message = 0xDC, checkvalue is 0x79
  //message = 0x683A, checkvalue is 0x7C
  //message = 0x4E85, checkvalue is 0x6B

  uint32_t remainder = (uint32_t)message_from_sensor << 8; //Pad with 8 bits because we have to add in the check value
  remainder |= check_value_from_sensor; //Add on the check value

  uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;

  for (int i = 0 ; i < 16 ; i++) //Operate on only 16 positions of max 24. The remaining 8 are our remainder and should be zero when we're done.
  {
    //Serial.print("remainder: ");
    //Serial.println(remainder, BIN);
    //Serial.print("divsor:    ");
    //Serial.println(divsor, BIN);
    //Serial.println();

    if( remainder & (uint32_t)1<<(23 - i) ) //Check if there is a one in the left position
      remainder ^= divsor;

    divsor >>= 1; //Rotate the divsor max 16 times so that we have 8 bits left of a remainder
  }

  return (byte)remainder;
}
