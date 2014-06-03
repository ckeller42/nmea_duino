// -*- mode: c++ -*-
/**************************************************************************
**       Title: 
**    $RCSfile: abbrev_defs,v $
**   $Revision: 1.1 $$Name:  $
**       $Date: 2008/05/26 15:44:44 $
**   Copyright: $Author: ckeller $
** Description:
**
**    
**
**-------------------------------------------------------------------------
**
**  $Log: abbrev_defs,v $
**
**
**************************************************************************/
#include <Arduino.h>
#include <SoftwareSerial.h>

//#include <HC05.h>
#include "TinyGPS++.h"

/*-------------------------------------------------------------------------
 *  Change this for debugging
 *-------------------------------------------------------------------------*/
//#define THEINPUTSERIAL nmeaSerial
#define THEINPUTSERIAL Serial

/*-------------------------------------------------------------------------
 *  Make sure you are connected to the correct pins
 *-------------------------------------------------------------------------*/
SoftwareSerial btNmeaSerial(6, 7); // RX, TX
SoftwareSerial nmeaSerial(10,12); // RX, TX


int debug=1;
bool haveFilname = 0;

TinyGPSPlus gps;


void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  Serial.println("# DEBUGING NMEA_Duino");

  // set the data rate for the SoftwareSerial port
  btNmeaSerial.begin(9600);
  nmeaSerial.begin(4800); // seen:
}







/*======================================================================*/
/*! 
 *   Forward data from the default serial port to the bluetooth 
 *   serial port
 */
/*======================================================================*/
void Forward()
{
  int incomingByte = 0;	// for incoming serial data
  
  if (THEINPUTSERIAL.available())
  {
    incomingByte = THEINPUTSERIAL.read();
    btNmeaSerial.write(incomingByte);
    Serial.write(incomingByte);
  }
}


bool haveDate = false;
bool haveTime = false;

void displayInfo()
{

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    haveDate = true;
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
    haveTime = true;
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  
  haveFilname = haveTime && haveDate;
  
  Serial.println();
}




/*======================================================================*/
/*! 
 *   
 */
/*======================================================================*/


/*======================================================================*/
/*! 
 *   Main Loop
 */
/*======================================================================*/
void loop() // run over and over
{

  if ( haveFilname == false)
  {
    while (THEINPUTSERIAL.available() >0)
    {
      if (gps.encode(THEINPUTSERIAL.read()))
      {
        displayInfo();
        haveFilname =true;
        break;
      }
    }
  }
  else
  {
    // directly send all the data via bluetooth
    Forward();
  }
}

