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
#include <SD.h>

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
SoftwareSerial g_BtNmeaSerial(2,3); // RX, TX
SoftwareSerial g_NmeaSerial(4,5); // RX, TX

/*-------------------------------------------------------------------------
 * File handling
 *-------------------------------------------------------------------------*/
const int g_chipSelect = 4;
TinyGPSPlus g_Gps; // nmea parser

char g_fname[12]; // filname for writing to sd
char g_foldername[10]; // g_foldername for writing to sd
bool g_haveFilename = false; // has the filname been set?
char g_full_filename[256];


bool g_SDOk=false; // SD is 
int g_counter_flush; //counter to wait for flush
const int g_max_to_flush = 1000; // then we flush the file
File g_DataFile; //file handle





void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  Serial.println("# DEBUGING NMEA_Duino");

  // set the data rate for the SoftwareSerial port
  g_BtNmeaSerial.begin(9600);

  g_NmeaSerial.begin(4800); // seen:

  if (not g_NmeaSerial.isListening())
    {
      Serial.println("# NMEA is not listening! :-(");
    }

  // setup sdcard
  g_counter_flush = 0;

  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(g_chipSelect)) 
    {
      Serial.println("Card failed, or not present! :-(");
      // don't do anything more:
      g_SDOk = false;
    }
  else
    {
      g_SDOk = true;
      Serial.println("card initialized. :-)");
    }
}

/*======================================================================*/
/*! 
 *  Write a byte to the file. Flush if the byte counter reaches max
 */
/*======================================================================*/
void storeByteToSd(int incomingByte)
{
  if (g_SDOk && g_DataFile.available())
    {
      g_DataFile.write(incomingByte);
      ++g_counter_flush;
      if (g_counter_flush > g_max_to_flush)
	{
	  g_DataFile.flush();
	  g_counter_flush = 0;
	}
    }
  else
    {
      //	Serial.println("Can not save to file! :-(");
    }
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
      g_BtNmeaSerial.write(incomingByte);
      //Serial.write(incomingByte);
      //Serial.print(".");
      storeByteToSd(incomingByte);
    }
}

/*======================================================================*/
/*! 
 *   Get a folder and filname
 */
/*======================================================================*/
void getFilename()
{

  if (g_Gps.date.isValid() && g_Gps.time.isValid())
    {
      snprintf(g_foldername,sizeof(g_foldername),
	       "%04d%02d%02d",
	       g_Gps.date.year(),
	       g_Gps.date.month(),
	       g_Gps.date.day());
    
      snprintf(g_fname, sizeof(g_fname),
	       "%02d%02d%02d.txt",
	       g_Gps.time.hour(),
	       g_Gps.time.minute(),
	       g_Gps.time.second());
    
      g_haveFilename = true;
      Serial.println(g_fname);    
      Serial.println(g_foldername);    
      
      snprintf(g_full_filename, sizeof(g_full_filename),
	       "%s/%s",
	       g_foldername,g_fname);
      Serial.println(g_full_filename);    
    
    }
}

/*======================================================================*/
/*! 
 *   Create folder with current date and open file with current time
 */
/*======================================================================*/
void doFileAndFolder()
{
  if (not g_SDOk)
    {
      Serial.println("SD not ready not folder and file :-(");
      return;
    }
  if ( not SD.mkdir(g_foldername) )
    {
      Serial.println("Error creating dir! :-( ");
      return;
    }
  
  g_DataFile = SD.open(g_full_filename,FILE_WRITE);
  //g_DataFile = SD.open(g_fname);
  if (not g_DataFile)
    {
      Serial.println("Error opening file! :-( ");
    }
}




/*======================================================================*/
/*! 
 *   Main Loop
 */
/*======================================================================*/
void loop() // run over and over
{

  // get enough data to generate a filename
  while (g_haveFilename == false &&THEINPUTSERIAL.available() >0 )
    {
      if (g_Gps.encode(THEINPUTSERIAL.read()))
	{
	  getFilename(); 
	}
    }

  // hurry we have a filename. open the file
  if ( g_haveFilename == true && not g_DataFile)
    {
      doFileAndFolder();
    }
      

  if ( g_haveFilename == true)
    {
      // directly send all the data via bluetooth
      Forward();
    }
  if (g_SDOk && g_DataFile)
    {
      // fluhing is handled by a counter
      //g_DataFile.flush();
    }
  
}

// -*- mode: c++ -*-
