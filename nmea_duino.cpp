/**************************************************************************
 **       Title: nmea_duino.cpp
 **
 ** Description: 
 **
 **    
 **
 **-------------------------------------------------------------------------
 **
 **************************************************************************/
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SD.h>

//#include <HC05.h>
#include "TinyGPS++.h"



/* Wiring:
   Micro-SD breakout board -> Arduino Nano V3
   CS  -> D10
   DI  -> D11
   DO  -> D12
   CLK -> D13
   GND -> GND
   5V  -> 5v
*/

/* Wiring:
   HC-05 -> Arduino Nano V3
   RXD -> D9
   TXD -> D8
   5V  -> 5V
   GND -> GND
*/

/* Wiring:
   Max485 Breakout Board -> Arduino Nano V3
   DE -> D7
   RE -> D7
   RO -> D5
   DI -> D6
*/



/*-------------------------------------------------------------------------
 *  Change this for debugging
 *-------------------------------------------------------------------------*/
#define THEINPUTSERIAL g_NmeaSerial
//#define THEINPUTSERIAL Serial

/*-------------------------------------------------------------------------
 * Bluetooth Serial Device
 *-------------------------------------------------------------------------*/
SoftwareSerial g_BtSerial(8,9); // RX, TX


/*-------------------------------------------------------------------------
 * Max485 Settings 
 *-------------------------------------------------------------------------*/
SoftwareSerial g_NmeaSerial(5,6); // RX, TX
#define SSerialTxControl 7   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

   
/*-------------------------------------------------------------------------
 * SD Card
 *-------------------------------------------------------------------------*/   
const int g_chipSelect = 10;
Sd2Card card;
SdVolume volume;
SdFile root;

bool g_SDOk=false; // SD is 
int g_counter_flush; //counter to wait for flush
const int g_max_to_flush = 1000; // then we flush the file
File g_DataFile; //file handle




/*-------------------------------------------------------------------------
 * File handling
 *-------------------------------------------------------------------------*/
TinyGPSPlus g_Gps; // nmea parser

char g_fname[12]; // filname for writing to sd
char g_foldername[10]; // g_foldername for writing to sd
bool g_haveFilename = false; // has the filname been set?
char g_full_filename[256];


void cardInfo();


void setupMax485()
{
  // Setup the max 485
  pinMode(SSerialTxControl, OUTPUT);  
  digitalWrite(SSerialTxControl, RS485Receive);  // Set to receiving
  g_NmeaSerial.begin(9600); // 

  delay(10);
  if (not g_NmeaSerial.isListening())
  {
    Serial.println("# NMEA is not listening! :-(");
  }
}


void setupSD()
{
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


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("# DEBUGING NMEA_Duino");
  
  //cardInfo();
    
  // set the data rate for the SoftwareSerial port
  g_BtSerial.begin(9600);

  setupMax485();
  setupSD();
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
    Serial.println("Can not save to file! :-(");
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
    g_BtSerial.write(incomingByte);
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
 *   Main Loop, run over and over
 */
/*======================================================================*/
void loop() 
{
  //g_BtSerial.println("Hurray");
  //Serial.println(".");
  //Forward();
  //g_NmeaSerial.println("AHAHHHA");
  
  // if ( g_NmeaSerial.available())
  // {

  //   int the_byte = g_NmeaSerial.read();
  //   g_BtSerial.write(the_byte);
  //   Serial.print(the_byte);
  // }
  // return;
  

  
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
}



void cardInfo() {


  Serial.println("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, g_chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


// -*- mode: c++ -*-
