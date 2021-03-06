cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME termoSender)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11")
generate_arduino_library(RF24
        BOARD uno
        SRCS /usr/share/arduino-1.6.9/libraries/RF24/RF24.cpp
        HDRS /usr/share/arduino-1.6.9/libraries/RF24/RF24.h
        )
generate_arduino_library(HP03
        BOARD uno
        SRCS /usr/share/arduino-1.6.9/libraries/HP03/src/HP03.cpp
        HDRS /usr/share/arduino-1.6.9/libraries/HP03/src/HP03.h
        )
generate_arduino_library(SD
        BOARD uno
        SRCS /usr/share/arduino-1.6.9/libraries/SD/src/SD.cpp /usr/share/arduino-1.6.9/libraries/SD/src/File.cpp
        HDRS /usr/share/arduino-1.6.9/libraries/SD/src/SD.h
        )
generate_arduino_library(SPI
        BOARD uno
        SRCS /usr/share/arduino-1.6.9/hardware/arduino/avr/libraries/SPI/src/SPI.cpp
        HDRS /usr/share/arduino-1.6.9/hardware/arduino/avr/libraries/SPI/src/SPI.h
        )
set(${CMAKE_PROJECT_NAME}_BOARD uno)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
set(${CMAKE_PROJECT_NAME}_LIBS RF24 HP03 SPI SD)
set(${CMAKE_PROJECT_NAME}_SKETCH termoSender.ino)
include_directories(
        /usr/share/arduino-1.6.9/libraries/RF24
        /usr/share/arduino-1.6.9/libraries/nFR24L01
        /usr/share/arduino-1.6.9/libraries/HP03/src
)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
