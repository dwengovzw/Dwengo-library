/****************************************************************************************	
**  This is example LINX firmware for use with the Dwenguino with the serial 
**  interface enabled.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
**  Written By Sam Kristoff
**  Modified on Mar 10 2015 by Jelle Roets from Dwengo vzw (www.dwengo.org)
**
**  BSD2 License.
****************************************************************************************/

//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>


//Include Device Sepcific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxDwenguino.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxDwenguino* LinxDevice;

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxDwenguino();
  
  // Custom dwenguino commands
  LinxSerialConnection.AttachCustomCommand(0x00, readPullup);

  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);  
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}

int readPullup(unsigned char numPins, unsigned char* pins, unsigned char* numResponseBytes, unsigned char* values)
{
	unsigned char bitOffset = 8;
	unsigned char byteOffset = 0;
	unsigned char retVal = 0;
 
	//Loop Over Pins To Read
	for(int i=0; i<numPins; i++)
	{
		//If bitOffset Is 0 We Have To Start A New Byte, Store Old Byte And Increment OFfsets
		if(bitOffset == 0)
		{
			//Insert retVal Into Response Buffer
			values[byteOffset] = retVal;
			retVal = 0x00;
			byteOffset++;
			bitOffset = 7;      
		}
		else
		{
			bitOffset--;
		}
		
		//Read From Next Pin
		unsigned char pinNumber = pins[i];
			
		pinMode(pinNumber, INPUT_PULLUP); //Set Pin As Input Pull up    		
		retVal = retVal | (digitalRead(pinNumber) << bitOffset);	//Read Pin And Insert Value Into retVal
	}
	
	//Store Last Byte
	values[byteOffset] = retVal;

	//Set number of send bytes
	*numResponseBytes = byteOffset + 1;

	return L_OK;
}
