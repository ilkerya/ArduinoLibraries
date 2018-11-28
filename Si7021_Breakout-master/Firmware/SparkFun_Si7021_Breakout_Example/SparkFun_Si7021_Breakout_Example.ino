/******************************************************************************
  SparkFun Si7021 Breakout Example 
  Joel Bartlett @ SparkFun Electronics
  Original Creation Date: May 18, 2015
  Updated May 4, 2016
  This sketch prints the temperature and humidity the Serial port.

  The library used in this example can be found here:
  https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries

  Hardware Connections:
      HTU21D ------------- Photon
      (-) ------------------- GND
      (+) ------------------- 3.3V (VCC)
       CL ------------------- D1/SCL
       DA ------------------- D0/SDA

  Development environment specifics:
  	IDE: Particle Dev
  	Hardware Platform: SparkFun RedBoard
                       Arduino IDE 1.6.5

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*******************************************************************************/
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <MICS-VZ-89TE.h>
#include "utility/twi.h"

#define TCAADDR 0x71 // A0 short
void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
    //dafruit Industries
    //  https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexerbreakout
    Wire.write(1 << i);
    Wire.endTransmission();
}


float humidity = 0;
float temperature = 0;


int power = A3;
int GND = A2;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;
MICS_VZ_89TE VOCSensor;


//---------------------------------------------------------------
void setup()
{
    Serial.begin(115200);   // open serial over USB at 9600 baud
        delay(200);
    Serial.println("Start");

    Wire.begin();
        
     //     digitalWrite(SDA, 0);
    //  digitalWrite(SCL, 0);

  //  pinMode(power, OUTPUT);
  //  pinMode(GND, OUTPUT);


  //  digitalWrite(GND, LOW);

    //Initialize the I2C sensors and ping them

/*
  Serial.println("\nTCAScanner ready!");
  for (uint8_t t=0; t<8; t++) {
  tcaselect(t);
  uint8_t addr;
  Serial.print("TCA Port #"); Serial.println(t);
      for ( addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
        uint8_t data;
        
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
          Serial.print("Found I2C 0x"); Serial.println(addr,HEX);
      }
      
    }
  }
  Serial.println("\ndone");
*/



    //  digitalWrite(power, HIGH);

        tcaselect(7);
    sensor.begin();
    delay(100);


        tcaselect(2);
        VOCSensor.begin();

    VOCSensor.getVersion();

}
//---------------------------------------------------------------
void loop()
{
    //Get readings from all sensors

//    sensor.begin();
 //   delay(100);
      tcaselect(7);
    getWeather();
    printInfo();

      tcaselect(2);
        VOCSensor.begin();
    VOCSensor.getVersion();
    VOCSensor.readSensor();

         float VOC = VOCSensor.getVOC();
         Serial.print("VOC:"); 
         Serial.print(VOC);

              float CO2 = VOCSensor.getCO2();
              
         Serial.print("    eCO2:"); 
         Serial.println(CO2);  

               Serial.print("Status:"); 

      float Status = VOCSensor.getStatus();
      Serial.println(Status);
    
  //  sensor.reset();
    delay(1000);
   


}
//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();
  delay(20);

  // Measure Temperature from the HTU21D or Si7021
  temperature = sensor.getTemp();
  if (temperature < -35){
        // Wire.endTransmission();
        /*
      digitalWrite(SDA, 1);
      digitalWrite(SCL, 1);
      delay(10);
      digitalWrite(SDA, 0);
      digitalWrite(SCL, 0);
delay(10);
       digitalWrite(SDA, 1);
      digitalWrite(SCL, 1);     
        */
    Serial.println("Error");

      delay(100);
 
      digitalWrite(power, HIGH);
      delay(100);
      sensor.begin();
    delay(100);


  
  }
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
//---------------------------------------------------------------
void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(temperature);
  Serial.print("C, ");

  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
}
