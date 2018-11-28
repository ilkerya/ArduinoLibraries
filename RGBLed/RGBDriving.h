#ifndef	RGB_DRIVING_H
#define	RGB_DRIVING_H
 
#include <Arduino.h>
 
 
#define	RED_PIN			11
#define GREEN_PIN		5
#define BLUE_PIN 		6

#define COLOR_1			0
#define COLOR_2			1
#define COLOR_3			2
#define COLOR_4			3
#define COLOR_5			4
#define COLOR_6			5
#define COLOR_7			6
#define COLOR_8			7
#define COLOR_9			8
#define COLOR_10		9
 
class RGBDrv
{
public:
	RGBDrv();
	
	void DispColor(unsigned char ColorNum);
	
	
	
	
	
private:
	unsigned char PinR;
	unsigned char PinG;
	unsigned char PinB;
	
};
 
 
 
 
 
 
 #endif
 
