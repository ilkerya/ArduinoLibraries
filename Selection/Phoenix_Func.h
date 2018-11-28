

#include <SoftwareSerial.h>

struct{
    uint16_t CO2;
    uint8_t statu;
    int32_t Resistance;
    uint16_t TVOC;
}IAQ;

struct{
  bool TimoeutStart;
  int TimoeutFollower;// = TIMEOUT;
  unsigned int Buzzer;
  unsigned int Fan;
}Timer;




AMS_IAQ_CORE_C iaqcore;




void IAQSensorApp(void){
	Serial.print("IAQ: ");
 if(!iaqcore.update()){
    Serial.println("Update Failed!");
  }

  for(uint8_t ii = 0; ii < 10; ii++){
   // Serial.print(".");
    if(iaqcore.data[ii] < 16){
      Serial.print(".");
    }
    Serial.print(iaqcore.data[ii], HEX);

    if(ii != 9){
     // Serial.print(", ");
    }
  }
  //Serial.println();
	Serial.print(" ");
	int Status = 44;
  switch(iaqcore.getStatus()){
  case AMS_IAQ_CORE_C_STATUS_BUSY:
    Serial.print("Busy ");
    break;
  case AMS_IAQ_CORE_C_STATUS_OK:
    Serial.print("OK ");
    Status =  33;
    break;
  case AMS_IAQ_CORE_C_STATUS_ERROR:
    Serial.print("Error ");
    break;
  case AMS_IAQ_CORE_C_STATUS_WARMING_UP:
    Serial.print("Warming Up ");
    break;
  default:
    Serial.print("Undefined Code: ");
 //   Serial.println(iaqcore.getStatus(), HEX);
    break;
  }
 IAQ.Resistance = iaqcore.getResistanceOhms();
 IAQ.CO2 = iaqcore.getCO2EquivalentPPM() - 450 ;   // base 450
 IAQ.TVOC = iaqcore.getTVOCEquivalentPPB() -125 ; // base 125
  //    Serial.println("Resistance [ohms]: " + String(IAQ.Resistance));
  //    Serial.println("CO2 Equivalent [ppm]: " + String(IAQ.CO2));
  //    Serial.println("TVOC Equivalent [ppb]: " +  String(IAQ.TVOC));
//      Serial.println("Resistance [ohms]: CO2 Equivalent [ppm]: TVOC Equivalent [ppb]: ");
      Serial.println(String(IAQ.Resistance)+ "     " + String(IAQ.CO2)+ "     "  +  String(IAQ.TVOC));


      if(Status ==  44){
		  IAQ.CO2 = 0;
		  IAQ.TVOC = 0;
	  }
}

bool TimeoutFunc(int Val) {
     if(!Timer.TimoeutStart){
        Timer.TimoeutStart = 1;
        Timer.TimoeutFollower = Val;
     }
      else Timer.TimoeutFollower--;
      if(!Timer.TimoeutFollower){
        Serial.println("Timeout: " + String(Val,DEC)+ "mSec");
          Timer.TimoeutStart = 0;
          return 1;
      }
      else return 0;
}

#define AC_OFFSETHOME 1
#define AC_OFFSETLEAVE 3
#define AC_OFFSETCOME 8


#define PM_OFFSET1 10
#define PM_OFFSET2 30
#define PM_OFFSET3 50
#define PM_OFFSET4 70
#define PM_OFFSET5 90

#define IAQ_SET 1

#define HUM_OFFSET1 2
#define HUM_OFFSET2 6
#define HUM_OFFSET3 10

#define ATHOME 0
#define LEAVEHOME 1
#define COMEHOME 2

byte Geofencing =0;

struct {
  byte ACMode;
  byte ACFanLevel;
  byte ACTemperature;  //  NUMERIC VALUE
  byte Humidifier;
  byte Purifier;
  byte Robot;
  byte ACSetTemp;
  byte ACOutTemp;
}Appliance;

// Appliance.ACMode
#define AC_COOL_ATHOME 21
#define AC_COOL_LEAVE 25
#define AC_COOL_COMING 17
#define AC_HEAT_ATHOME 23
#define AC_HEAT_LEAVE 18
#define AC_HEAT_COMING 25

// Appliance.ACMode
#define AC_OFF 0
#define AC_COOL 1
#define AC_HEAT 2
#define AC_DEHUM 3
#define AC_AUTO 4  // FUTURE USE

// Appliance.ACFanLevel
#define FAN_OFF 0
#define FAN_LOW 1
#define FAN_MID 2
#define FAN_HIGH 3

// Appliance.Humidifier
#define HUM_OFF 0
#define HUM_LOW 1
#define HUM_MID 2
#define HUM_HIGH 3

// Appliance.Purifier
#define PUR_OFF 0
#define PUR_LOW 1
#define PUR_MID 2
#define PUR_HIGH 3

// Appliance.Robot
#define ROBOT_OFF 0
#define ROBOT_ON 1



void SetAirCon(float Temperature){

  byte InTemp = (byte)Temperature; // FLOAT TO BYTE
  Appliance.ACSetTemp = 23; // DEFAULT COMFORT
  Appliance.ACTemperature = Appliance.ACSetTemp; // ´ASSUMING IT AS SET

  switch(Geofencing){
    default:
    case ATHOME:
                    if (Appliance.ACSetTemp > (InTemp + AC_OFFSETHOME)) // heat
                    {
                        Appliance.ACMode = AC_HEAT; // heating
                        Appliance.ACFanLevel = FAN_HIGH;
                        Appliance.ACTemperature = AC_HEAT_ATHOME;
                    }
                    else if (Appliance.ACSetTemp < (InTemp - AC_OFFSETHOME)) // cool
                    {
                        Appliance.ACMode = AC_COOL; // cooling
                        Appliance.ACFanLevel = FAN_HIGH;
                        Appliance.ACTemperature = AC_COOL_ATHOME;
                    }
                    else // default off
                    {
                        Appliance.ACMode = AC_OFF; // off
                        Appliance.ACFanLevel = FAN_OFF;
                    }
    break;
     case LEAVEHOME:
                       if (Appliance.ACSetTemp > (InTemp + AC_OFFSETHOME)) // heat
                    {
                        Appliance.ACMode = AC_HEAT; // heating
                        Appliance.ACFanLevel = FAN_LOW;
                        Appliance.ACTemperature = AC_HEAT_LEAVE;
                    }
                    else if (Appliance.ACSetTemp < (InTemp - AC_OFFSETHOME)) // cool
                    {
                        Appliance.ACMode = AC_COOL; // cooling
                        Appliance.ACFanLevel = FAN_LOW;
                        Appliance.ACTemperature = AC_COOL_LEAVE;
                    }
                    else // default off
                    {
                        Appliance.ACMode = AC_OFF; // off
                        Appliance.ACFanLevel = FAN_OFF;
                    }
    break;
     case COMEHOME:
                    if (Appliance.ACSetTemp > (InTemp + AC_OFFSETHOME)) // heat
                    {
                        Appliance.ACMode = AC_HEAT; // heating
                        Appliance.ACFanLevel = FAN_MID;
                        Appliance.ACTemperature = AC_HEAT_COMING;
                    }
                    else if (Appliance.ACSetTemp < (InTemp - AC_OFFSETHOME)) // cool
                    {
                        Appliance.ACMode = AC_COOL; // cooling
                        Appliance.ACFanLevel = FAN_MID;
                        Appliance.ACTemperature = AC_COOL_COMING;
                    }
                    else // default off
                    {
                        Appliance.ACMode = AC_OFF; // off
                        Appliance.ACFanLevel = FAN_OFF;
                    }
    break;
  }

}


void SetHumidifier(float Humidity){
  byte InHumidity = (byte)Humidity; // FLOAT TO BYTE
  byte IdealHumidity = 34; //test

  switch(Geofencing){
    default:
    case ATHOME:
                    if ((IdealHumidity - HUM_OFFSET3) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_HIGH;
                    }
                    else if ((IdealHumidity - HUM_OFFSET2) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_MID;
                    }
                    else if ((IdealHumidity - HUM_OFFSET1) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_LOW;
                    }
                    else
                    {
                        Appliance.Humidifier= HUM_LOW;
                    }
    break;
     case LEAVEHOME:
                    if ((IdealHumidity - HUM_OFFSET3) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_LOW;
                    }
                    else if ((IdealHumidity - HUM_OFFSET2) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_OFF;
                    }
                    else if ((IdealHumidity - HUM_OFFSET1) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_OFF;
                    }
                    else
                    {
                        Appliance.Humidifier= HUM_OFF;
                    }
    break;
     case COMEHOME:
                    if ((IdealHumidity - HUM_OFFSET3) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_MID;
                    }
                    else if ((IdealHumidity - HUM_OFFSET2) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_LOW;
                    }
                    else if ((IdealHumidity - HUM_OFFSET1) > InHumidity)
                    {
                        Appliance.Humidifier= HUM_OFF;
                    }
                    else
                    {
                        Appliance.Humidifier= HUM_OFF;
                    }
    break;
  }

}

void SetPurifier(int VOC){

    int IAQ = VOC; // FLOAT TO BYTE
    int IAQ_Set = 60;
  switch(Geofencing){
    default:
    case ATHOME:
                     if ((IAQ_Set + PM_OFFSET5) < IAQ) // // use offsets
                    {
                        Appliance.Purifier = PUR_HIGH; //
                        Appliance.ACFanLevel = FAN_HIGH;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else  if ((IAQ_Set + PM_OFFSET4) < IAQ) //
                    {
                        Appliance.Purifier = PUR_HIGH; //
                        Appliance.ACFanLevel = FAN_HIGH;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET3) < IAQ) //
                    {
                        Appliance.Purifier = PUR_MID; //
                        Appliance.ACFanLevel = FAN_MID;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET2) < IAQ) //
                    {
                        Appliance.Purifier = PUR_MID; //
                        Appliance.ACFanLevel = FAN_LOW;
                        Appliance.Robot = ROBOT_OFF;
                    }
                    else if ((IAQ_Set + PM_OFFSET1) < IAQ) //
                    {
                        Appliance.Purifier = PUR_LOW; //
                        Appliance.ACFanLevel = FAN_LOW;

						Appliance.Robot = ROBOT_OFF;
                    }
                    else // default off
                    {
                        Appliance.Purifier = PUR_OFF; // off
                        if (Appliance.ACMode == AC_OFF) Appliance.ACFanLevel = FAN_OFF;
                        else Appliance.ACFanLevel = FAN_LOW;  // if cool or heat
                        Appliance.Robot = ROBOT_OFF;
                    }
    break;
     case LEAVEHOME:
                     if ((IAQ_Set + PM_OFFSET5) < IAQ) // // use offsets
                    {
                        Appliance.Purifier = PUR_MID; //
                        Appliance.ACFanLevel = FAN_LOW;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else  if ((IAQ_Set + PM_OFFSET4) < IAQ) //
                    {
                        Appliance.Purifier = PUR_LOW; //
                        Appliance.ACFanLevel = FAN_OFF;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET3) < IAQ) //
                    {
                        Appliance.Purifier = PUR_OFF; //
                        Appliance.ACFanLevel = FAN_OFF;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET2) < IAQ) //
                    {
                        Appliance.Purifier = PUR_OFF; //
                        Appliance.ACFanLevel = FAN_OFF;
                        Appliance.Robot = ROBOT_ON;
                    }
                    else if ((IAQ_Set + PM_OFFSET1) < IAQ) //
                    {
                        Appliance.Purifier = PUR_OFF; //
                        if (Appliance.ACMode == AC_OFF) Appliance.ACFanLevel = FAN_OFF;
                        else Appliance.ACFanLevel = FAN_LOW;  // if cool or heat
                        Appliance.Robot = ROBOT_ON;

                    }
                    else // default off
                    {
                        Appliance.Purifier = PUR_OFF; // off
                        if (Appliance.ACMode == AC_OFF) Appliance.ACFanLevel = FAN_OFF;
                        else Appliance.ACFanLevel = FAN_LOW;  // if cool or heat
                    }
    break;
     case COMEHOME:
                     if ((IAQ_Set + PM_OFFSET5) < IAQ) // // use offsets
                    {
                        Appliance.Purifier = PUR_HIGH; //
                        Appliance.ACFanLevel = FAN_MID;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else  if ((IAQ_Set + PM_OFFSET4) < IAQ) //
                    {
                        Appliance.Purifier = PUR_MID; //
                        Appliance.ACFanLevel = FAN_MID;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET3) < IAQ) //
                    {
                        Appliance.Purifier = PUR_LOW; //
                        Appliance.ACFanLevel = FAN_LOW;
                        Appliance.Robot = ROBOT_ON;

                    }
                    else if ((IAQ_Set + PM_OFFSET2) < IAQ) //
                    {
                        Appliance.Purifier = PUR_LOW; //
                        Appliance.ACFanLevel = FAN_OFF;
                        Appliance.Robot = ROBOT_ON;
                    }
                    else if ((IAQ_Set + PM_OFFSET1) < IAQ) //
                    {
                        Appliance.Purifier = PUR_OFF; //
                        if (Appliance.ACMode == AC_OFF) Appliance.ACFanLevel = FAN_OFF;
                        else Appliance.ACFanLevel = FAN_LOW;  // if cool or heat
                        Appliance.Robot = ROBOT_ON;

                    }
                    else // default off
                    {
                        Appliance.Purifier = PUR_OFF; // off
                        if (Appliance.ACMode == AC_OFF) Appliance.ACFanLevel = FAN_OFF;
                        else Appliance.ACFanLevel = FAN_LOW;  // if cool or heat
                        Appliance.Robot = ROBOT_OFF;
                    }

    break;
  }

}


	unsigned char Date[12] = __DATE__; // "Mmm dd yyyy"
	unsigned char Time[9] = __TIME__; // "hh:mm:ss"
struct{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned int Year;
	unsigned char Date;
	unsigned char Month[3];
}Compile_Info;  // Compile_Info.Minute

void Update_Compiler_Info(){
 	Compile_Info.Month[0] = Date[0];
 	Compile_Info.Month[1] = Date[1];
 	Compile_Info.Month[2] = Date[2];
	Compile_Info.Date = (Date[4]-48)*10 + (Date[5]-48);
 	Compile_Info.Year = (Date[7]-48)*1000 + (Date[8]-48)*100 + (Date[9]-48)*10 + (Date[10]-48) ;
  	Compile_Info.Hour =   (Time[0]-48)*10 + (Time[1]-48);
  	Compile_Info.Minute = (Time[3]-48)*10 + (Time[4]-48);
  	Compile_Info.Second = (Time[6]-48)*10 + (Time[7]-48);
}
void CompilerInfo(void){
	Update_Compiler_Info();
	Serial.print("Compile Time: ");
	Serial.print(Compile_Info.Hour);Serial.print(':');
	Serial.print(Compile_Info.Minute);Serial.print(':');
	Serial.println(Compile_Info.Second);
	Serial.print("        Date: ");
	Serial.print((char)Date[4]);
	Serial.print((char)Date[5]);
	Serial.print('.');
	Serial.print((char)Date[0]);
	Serial.print((char)Date[1]);
	Serial.print((char)Date[2]);Serial.print('.');
	Serial.println(Compile_Info.Year);
}


