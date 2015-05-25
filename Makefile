### ARDUINO_DIR
### Path to the Arduino application and resources directory.
### On OS X:
#ARDUINO_DIR       = /Applications/Arduino.1.6.0.app/Contents/Java


### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/cu.usbserial-*
MONITOR_BAUDRATE = 9600 


### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
BOARD_TAG         = nano
BOARD_SUB = atmega328


### PROJECT_DIR
### This is the path to where you have created/cloned your project
PROJECT_DIR       = ./


### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile


### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(PROJECT_DIR)/libraries



ARDUINO_LIBS = SoftwareSerial TinyGPSPlus SD SPI


### Do not touch - the path to Arduino.mk, inside the ARDMK_DIR
include $(ARDMK_DIR)/Arduino.mk

