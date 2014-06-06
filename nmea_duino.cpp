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
SoftwareSerial btNmeaSerial(10,11); // RX, TX
SoftwareSerial nmeaSerial(8,9); // RX, TX




char fname[12]; // filname for writing to sd
char foldername[10]; // foldername for writing to sd
bool haveFilname = false; // has the filname been set?




TinyGPSPlus gps; // nmea parser


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
    //Serial.print(".");
  }
}


/*======================================================================*/
/*! 
 *   Get a folder and filname
 */
/*======================================================================*/
void getFilename()
{

  if (gps.date.isValid() && gps.time.isValid())
  {
    snprintf(foldername,sizeof(foldername),
             "%04d%02d%02d",
             gps.date.year(),
             gps.date.month(),
             gps.date.day());
    
    snprintf(fname, sizeof(fname),
             "%02d%02d%02d.txt",
             gps.time.hour(),
             gps.time.minute(),
             gps.time.second());
    
    haveFilname = true;
    Serial.println(fname);    
    Serial.println(foldername);    
  }
}




/*======================================================================*/
/*! 
 *   Main Loop
 */
/*======================================================================*/
void loop() // run over and over
{

  
  while (THEINPUTSERIAL.available() >0 && haveFilname == false)
  {
    if (gps.encode(THEINPUTSERIAL.read()))
    { getFilename(); }
  }
  
  if ( haveFilname == true)
  {
    // directly send all the data via bluetooth
    Forward();
  }
}

