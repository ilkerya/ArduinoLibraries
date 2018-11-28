#include <Arduino.h>
#include "RGBDriving.h"

//--------------------------------------------------------
//R,G,B driving pin has been defined to pin11,pin5,pin6
//
//There are about 10 defined color, in mainloop call RGBDrv::DispColor(unsigned char ColorNum) can change to the defined color.
//                                        //for example,RGBLed1.DispColor(COLOR_1)
//color could be change by changing the pwm value of each driving pin.
//--------------------------------------------------------
//--------------------------------------------------------
RGBDrv::RGBDrv()
{
	PinR = RED_PIN;
	PinG = GREEN_PIN;
	PinB = BLUE_PIN;

	pinMode(PinR, OUTPUT);
	pinMode(PinG, OUTPUT);
	pinMode(PinB, OUTPUT);
}
//--------------------------------------------------------
void RGBDrv::DispColor(unsigned char ColorNum)
{
	unsigned char ValueR = 0;
	unsigned char ValueG = 0;
	unsigned char ValueB = 0;

	if (ColorNum == 0)
	{
		ValueR = 125;
		ValueG = 125;
		ValueB = 125;
	}
	else if (ColorNum == 1)//pure green
	{
		ValueR = 0;
		ValueG = 250;
		ValueB = 0;
	}
	else if (ColorNum == 2)//pure blue
	{
		ValueR = 0;
		ValueG = 0;
		ValueB = 250;
	}
	else if (ColorNum == 3)//green1
	{
		ValueR = 0;
		ValueG = 200;
		ValueB = 50;
	}
	else if (ColorNum == 4)// blue
	{
		ValueR = 0;
		ValueG = 50;
		ValueB = 200;
	}
	else if (ColorNum == 5)//green2
	{
		ValueR = 0;
		ValueG = 150;
		ValueB = 100;
	}
	else if (ColorNum == 6) //red
	{
		ValueR = 100;
		ValueG = 0;
		ValueB = 50;
	}
	else if (ColorNum == 7)//red
	{
		ValueR = 200;
		ValueG = 0;
		ValueB = 50;
	}
	else if (ColorNum == 8)
	{
		ValueR = 125;
		ValueG = 0;
		ValueB = 125;
	}
	else if (ColorNum == 9) // yellow
	{
		ValueR = 250;//125
		ValueG = 250;// 0
		ValueB = 0; // 20
	}
	else if (ColorNum == 10)// purple
	{
		ValueR = 125; //0
		ValueG = 0; //250
		ValueB = 125;
	}
	else
	{
		ValueR = 250; // 200
		ValueG = 0;// 125
		ValueB = 0;
	}

	analogWrite(PinR,ValueG);
	analogWrite(PinG,ValueB);
	analogWrite(PinB,ValueR);
}
//--------------------------------------------------------

//--------------------------------------------------------

