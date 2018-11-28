
#include <Wire.h>
#include "TouchSensor.h"


//-------------------------------------
TouchSensor::TouchSensor()
{
     pinMode(TOUCH_KEY_PIN, INPUT);
}


//-------------------------------------
unsigned char TouchSensor::ReadKey(void)
{
	unsigned char KeyValue = 0;
	
	 if (digitalRead(TOUCH_KEY_PIN) == LOW)
	 {
		KeyValue = 1;	
		 
	 }
	 else
	 { 
		 KeyValue = 0;
	 }
	 
	 return(KeyValue);
}
 