// -*- mode: c++ -*-
/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>
//#include <TinyGPS++.h>

// The TinyGPS++ object
//TinyGPSPlus gps;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


SoftwareSerial mySerial(10, 11); // RX, TX


void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  */

  Serial.println("Serial available");

  // set the data rate for the SoftwareSerial por// 
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
    else
    {
      inputString += inChar;
    }
  }
}
*/

/*
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString); 
    mySerial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
*/

void loop() // run over and over
{
  if (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
  
  if (Serial.available())
  {
    mySerial.write(Serial.read());
  }
}

