#include "IRRemote.h"

//-------------------------------------------------------
//                                        _______________
//Lead Code 4.5ms pwm + 4.5ms mute                       |________________
//                                        ____
//Bit 1 code: 0.54ms pwm + 1.68ms mute        |________
//                                        ____    
//Bit 0 code: 0.54ms pwm + 0.54ms mute        |___
//                                       
//inter mute  5ms                         |__________________|
//                                         ____    
//end code    0.54ms pwm                  |    |
//
//A Frame data = Leadcode A +~A + B +~B + C + ~C + Endcode + InterMute + Leadcode + A +~A + B +~B + C + ~C +Endcode
//
//--description of public function
//
//IRRemoteSend::Begin()														//should be initialize in setup()
//
//IRRemoteSend::StartCommand(unsigned char AcType,unsigned char Command)	//when call this in mainloop, a command would be sent.
//                                               							//For example IRSend1.StartCommand(AC_ELUX,TURN_ON); 
//IRRemoteSend::ISRPrc()		//Since timer4 has been used for counting the width of each code, this fuction should run in interrupt service route of timer4
//
//IRRemoteSend::PwmOn()			//timer1 has been used for generating 38khz pwm 
//-------------------------------------------------------

//--------------------declaration
void InitTime4();
void InitTime4ForIR();


//--------------------------------------------------------
IRRemoteSend::IRRemoteSend()			
{	
	//-----set the physical time
	
	SendBit = 0;	 
		
	LeadCodeHighWidth = LEAD_CODE_HIGH_WIDTH;
	LeadCodeLowWidth = LEAD_CODE_LOW_WIDTH;
	
	BitCodeHighWidth = BIT_CODE_HIGH_WIDTH;
	Bit1CodeLowWidth = BIT1_CODE_LOW_WIDTH;
	Bit0CodeLowWidth = BIT0_CODE_LOW_WIDTH;
	
	EndCodeWidth = END_CODE_WIDTH;
	
	InterMuteWidth = INTER_MUTE_WIDTH;
}
//--------------------------------------------------------
void IRRemoteSend::Begin()
{
	InitTime4();
	
}
//--------------------------------------------------------
void IRRemoteSend::StartCommand(unsigned char AcType,unsigned char Command)
{	
	if (SendStatus == 0)				//only start while there is no transmitting
	{
		IRsending = 1;
		
		
		//EnableTimer4Int();
		
		MaxSendBit = SetTxData(AcType,Command);
		
	
		//TCNT4 = (255 - LeadCodeHighWidth);
		//OCR4C = LeadCodeHighWidth;
		//TCNT4 = 0;
		InitTime4ForIR();
		MaxCount = LeadCodeHighWidth;
		
		PwmOn();	 
	 
		SendStatus = LEAD_CODE_HIGH_SENDING;
	
		SendBit = 0;
		IsSecondFrame = 0;
		//Serial.println("leadcode high");  
	}
	
}
//--------------------------------------------------------
//-A frame Data: Lead + A +~A + B +~B + C + ~C + End + InterMute + Lead + A +~A + B +~B + C + ~C +End
//--------------------------------------------------------
void IRRemoteSend::ISRPrc()
{
	unsigned char m = 0;
	unsigned char n = 0;
	unsigned char p = 0;
	
	TickCount++;
	if (TickCount >= MaxCount)
	{
		TickCount = 0;
		
		if (SendStatus == LEAD_CODE_HIGH_SENDING)
		{		
			//TCNT4 = (255 - LeadCodeLowWidth);
			MaxCount = LeadCodeLowWidth;
		
			PwmOff();
		
			SendStatus = LEAD_CODE_LOW_SENDING;
		}
		else if (SendStatus == LEAD_CODE_LOW_SENDING)
		{
			//TCNT4 = (255 - BitCodeHighWidth);
			MaxCount = BitCodeHighWidth;
		
			PwmOn();
		
			SendStatus = DATA_BIT_HIGH_SENDING;
		}
		else if (SendStatus == DATA_BIT_HIGH_SENDING)
		{
			p = (SendBit >> 3);				// div 8
			n = SendBit & 0x07;							// get reminder
				
			if ((TxBuff[p] & (0x80 >> n)) != 0)
			{			
				//--is bit1
				//TCNT4 = (255 - Bit1CodeLowWidth);
				MaxCount = Bit1CodeLowWidth;
			
			}
			else
			{
				//--is bit0
				//TCNT4 = (255 - Bit0CodeLowWidth);
				MaxCount = Bit0CodeLowWidth;
			}
		
			PwmOff();
		
			//--set the low bit 		
			SendStatus = DATA_BIT_LOW_SENDING;
		}
		else if (SendStatus == DATA_BIT_LOW_SENDING)
		{		
			SendBit++;
			if (SendBit >=  MaxSendBit)					//next is end code
			{
				SendStatus = END_CODE_SENDING;
				//TCNT4 = (255 - EndCodeWidth);
				MaxCount = EndCodeWidth;
			}
			else
			{			
				SendStatus = DATA_BIT_HIGH_SENDING;
				//TCNT4 = (255 - BitCodeHighWidth);		
				MaxCount = BitCodeHighWidth;
			}
		
			PwmOn();
		}
		else if (SendStatus == END_CODE_SENDING)
		{
			SendStatus = INTER_MUTE_SENDING;
			//TCNT4 = (255 - INTER_MUTE_WIDTH);
			MaxCount = INTER_MUTE_WIDTH;
		
			PwmOff();
		}
		else if (SendStatus == INTER_MUTE_SENDING)
		{
			if (IsSecondFrame == 0)			//command only send half
			{
				SendStatus = LEAD_CODE_HIGH_SENDING;
		
				SendBit = 0;
				IsSecondFrame = 1;
			
				//TCNT4 = (255 - LeadCodeHighWidth);
				MaxCount = LeadCodeHighWidth;
				PwmOn();
			
			}
			else							//Command end
			{
				SendStatus = 0;			
			
				PwmOff();
			
				IRsending = 0;					//sending stop
				InitTime4();
			}
		
		}
	}
	
	
}


//--------------------------------------------------------
void IRRemoteSend::PwmOn()	//timer1 has been used for generating IR 38kHz pwm
{
	pinMode(IR_PIN, OUTPUT);
    cli();
    
    TCCR1A = 0x40;
    TCCR1B = 0x0A;
        
    
    OCR1A = 26;         //IR output carrier freq = 38khz value=(16MHz/div)/2/38,  Timer1
    sei();
}
//--------------------------------------------------------
void IRRemoteSend::PwmOff()
{
	TCCR1A &= 0x3F;      //clear COM0A0, clear OC0A
}


//--------------------------------------------------------
unsigned char IRRemoteSend::SetTxData(unsigned char AcType,unsigned char ACCommand)
{
	unsigned Txbitcnt = 0;
	
	if (AcType == AC_ELUX)
	{
		if (ACCommand == TURN_ON)
		{
			TxBuff[0] = 0xb2;
			TxBuff[1] = ~(TxBuff[0]);
			TxBuff[2] = 0xBF;
			TxBuff[3] = ~(TxBuff[2]);	
			TxBuff[4] = 0x2C;
			TxBuff[5] = ~(TxBuff[4]);						
			
			Txbitcnt = 8 * 6;
			
		}
		else if (ACCommand == TURN_OFF)
		{
			TxBuff[0] = 0xb2;
			TxBuff[1] = ~(TxBuff[0]);
			TxBuff[2] = 0x7b;
			TxBuff[3] = ~(TxBuff[2]);	
			TxBuff[4] = 0xe0;
			TxBuff[5] = ~(TxBuff[4]);	
					
			
			Txbitcnt = 8 * 6;
		}
	}
	else
	{
		MaxSendBit = 0;
	}
	
	return(Txbitcnt);
}
//--------------------------------------------------------
void InitTime4()
{
  
  TCCR4A = 0;             //disconnect pin
  TCCR4B = 0x0a;          //div = 512  1 clock = 32us
  TCCR4D = 0;             //normal mode
  
  OCR4C = 0xff;			  //8.192ms
  
  TIMSK4 = 0x04;
}
//--------------------------------------------------------
void InitTime4ForIR()
{
  
  //TCCR4A = 0;             //disconnect pin
  TCCR4B = 0x03;          //div = 4  1 clock = 0.25us
 // TCCR4D = 0;             //normal mode
  
  OCR4C = 255;			  //64us for 1 tick 255*0.25   //tips: OCR4C should be max make sure higher than the Blue Led pwm duty value .otherwise the pwm will not generate.
  
  //TIMSK4 = 0x04;
}
//--------------------------------------------------------

