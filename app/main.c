#include "common.h"
#include "oled.h"

#include "Display.h"
#include "Timer.h"
#include "Uart.h"
#include "MAX30100.h"
//#include "Power.h"

unsigned char xdata SMode=0;

enum FucState
{
	TimeMode=1,
	HeartRateMode,
	StepMode
};

void main()
{
	
	unsigned char ID=0 ;

	P_SW2=0xb0;
	I2CCFG=0xe0;
	I2CMSST=0x00;

	// MAX30100_startTemperatureSampling();
	
	//	powerInit();

	Timer0Init();
	SMode=TimeMode;

	MAX30100_writeRegister(0x06,0x0b);
	MAX30100_writeRegister(0x01,0xf0);
	MAX30100_writeRegister(0x09,0x33);
	
	Uart2Init();
	Timer0Init();

	OLED_Init();

	displayHoursH(0);
	displayColon(0xff);
	displayMinuteH(0);
	displayIcon_Battery(2,0xff);
	displayIcon_Bluetooth(0x00);

	Uart2_SendStr("Started!\n");
	



	while(1)
	{


		while(SMode==TimeMode)
		{

				
			if(isSecondChange)
			{
				displayColon(0x00);
			
				isSecondChange=0;
			}

			if(TickCount==25)	displayColon(0xff);

			if(isMinuteChange)
			{
				displayMinuteL(Minute%10);
				if((Minute/10)!=preMinuteH)
				{ 
					preMinuteH=Minute/10;
					displayMinuteH(preMinuteH);
					
				}
				isMinuteChange=0;

			}
			if(isHourChange)
			{
				displayHoursL(Hour%10);
				if((Hour/10)!=preHourH) 
				{
					preHourH=Hour/10;
					displayHoursH(preHourH);
				}
				isHourChange=0;
			}


		
	//  getBatteryState();
	//  displayIcon_Battery(4,0xff);
		
	
		}

		while(SMode==HeartRateMode)
		{
			displayHRmodeIcon();
			if (Rptr != Wptr)
			{
				
				// ID=MAX30100_getPartID();
				// Uart2SendChar(buffer[Rptr++]);
				// Uart2SendChar(ID);
				switch(buffer[Rptr-1])
				{
					case 1 :  Uart2SendChar( MAX30100_Init() );break;
					case 2 :  Uart2SendChar( MAX30100_getPartID() );break;
					case 3 :  MAX30100_startTemperatureSampling();Uart2_SendStr("wait a moment then get temperature\n");break;
					case 4 :  Uart2_SendStr("integer"); Uart2SendChar(MAX30100_readRegister(ADDRESS_MAX30100_TEMP)); Uart2_SendStr("\n");break;
					case 6 :  shutdown(); break;
					case 7 :  resume(); break;
					default : break;
				}
				
				
				Rptr &= 0x0f;

			}
		

			
		}
		while(SMode==StepMode)
		{


		}


	}


}
