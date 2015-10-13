#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

void setup()
{
    initDwenguino();
    Serial1.begin(9600);    // Serial1 oepn communication with the Bluetooth module
                            // at 9600 baud rate.
    
    // Write the messages to show to the user.
    dwenguinoLCD.setCursor(0,0);
    dwenguinoLCD.print("Input: ");
    dwenguinoLCD.setCursor(0,1);
    dwenguinoLCD.print("Sum: ");
}

void loop()
{
      // Listen for data on the serial connection
    if ( Serial1.available() > 0 )
    {
       // Read in 2 real numbers
       float a = Serial1.parseFloat();
       float b = Serial1.parseFloat();
       // Print them to the screen after "Input"
       dwenguinoLCD.setCursor(6,0);
       dwenguinoLCD.print(a);
       dwenguinoLCD.print("+");
       dwenguinoLCD.print(b);
         
       // Add them together and return the result
       float sum = a + b;
       Serial1.println( sum );
       // Print the result to the LCD after "Sum"
       dwenguinoLCD.setCursor(5,1);
       dwenguinoLCD.print(sum);
   }
}
