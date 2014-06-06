#=============================================================================#
# Author: QueezyTheGreat                                                      #
# Date:   26.04.2011                                                          #
#                                                                             #
# Description: Arduino CMake example                                          #
#                                                                             #
#=============================================================================#
set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake) # Arduino Toolchain


cmake_minimum_required(VERSION 2.8)


#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(NMEA_Duino C CXX)


set(ARDUINO_CXX_FLAGS "${ARDUINO_CXX_FLAGS}")


set(FIRMWARE_NAME nmea_duino)
set(${FIRMWARE_NAME}_SRCS nmea_duino.cpp)

set(${FIRMWARE_NAME}_BOARD atmega328) # Default Board ID, when not specified
set(${FIRMWARE_NAME}_PORT /dev/ttyUSB0) # Default Port, when not specified


generate_arduino_library(TinyGPSPlus
  BOARD atmega328
  SRCS libraries/TinyGPSPlus/TinyGPS++.cpp
  HDRS libraries/TinyGPSPlus/TinyGPS++.h)

link_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/TinyGPSPlus)

generate_arduino_firmware(${FIRMWARE_NAME}
  LIBS TinyGPSPlus)
