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
#set(ARDUINO_CXX_FLAGS "${ARDUINO_CXX_FLAGS} -std=c++11")
set(ARDUINO_CXX_FLAGS "${ARDUINO_CXX_FLAGS}")
project(ArduinoExample C CXX)

#print_board_list()
#print_programmer_list()

set(ARDUINO_DEFAULT_BOARD atmega328) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)
set(TinyGPSPlus_RECURSE True)

#set(ARDUINO_DEFAULT_LIBRARY TinyGPSPlus) 





generate_arduino_firmware(nmea_duino
                          SKETCH . )
