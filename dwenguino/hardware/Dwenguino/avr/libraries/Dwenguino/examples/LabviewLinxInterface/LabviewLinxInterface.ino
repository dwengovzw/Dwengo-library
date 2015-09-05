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

//Include Dwenguino Libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

//Include Device Sepcific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxDwenguino.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxDwenguino* LinxDevice;

//Initialize LINX Device And Listener
void setup() {
  //Instantiate The LINX Device
  LinxDevice = new LinxDwenguino();
  
  // Custom dwenguino commands
  LinxSerialConnection.AttachCustomCommand(0x00, readPullup);
  LinxSerialConnection.AttachCustomCommand(0x01, writeLCD);

  // Initialise dwenguino
  dwenguinoLCD.begin(16,2);
  dwenguinoLCD.clear();

  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);  
}

void loop() {
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}

int readPullup(unsigned char numPins, unsigned char* pins, unsigned char* numResponseBytes, unsigned char* values) {
  unsigned char bitOffset = 8;
  unsigned char byteOffset = 0;
  unsigned char retVal = 0;
   
  //Loop Over Pins To Read
  for(int i=0; i<numPins; i++){
    //If bitOffset Is 0 We Have To Start A New Byte, Store Old Byte And Increment OFfsets
    if(bitOffset == 0){
      //Insert retVal Into Response Buffer
      values[byteOffset] = retVal;
      retVal = 0x00;
      byteOffset++;
      bitOffset = 7;      
    } else {
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

int writeLCD(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response){
  input[numInputBytes] = 0x00;  // add string termination char
  String str((char*)input);     // convert ascii array to string object
  
  pinMode(LCD_RS, OUTPUT);		// make sure command pins are output
  pinMode(LCD_RW, OUTPUT);
  pinMode(LCD_E, OUTPUT);
  dwenguinoLCD.clear();         // clear lcd
  
  int pos = min(numInputBytes, 16);
  int newline = str.indexOf('\n');
  if (newline > -1 && newline < 16) pos = newline;
  dwenguinoLCD.print(str.substring(0, pos));  // print first line of string: first 16 chars or until newline is found
  if (pos == newline) pos++;
  
  if (pos < numInputBytes){     // print second line if there more chars in the string
    int pos2 = min(numInputBytes, pos + 16);
    newline = str.indexOf('\n', pos);
    if (newline > -1 && newline < pos + 16) pos2 = newline;
    
    dwenguinoLCD.setCursor(0,1);
    dwenguinoLCD.print(str.substring(pos, pos2));
  }

  *numResponseBytes = 0;        // don't send a response
   
  return L_OK;
}