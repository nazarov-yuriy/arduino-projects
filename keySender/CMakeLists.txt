cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME keySender)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11")
generate_arduino_library(HID
        BOARD leonardo
        SRCS  /usr/share/arduino-1.6.9/hardware/arduino/avr/libraries/HID/src/HID.cpp
        HDRS /usr/share/arduino-1.6.9/hardware/arduino/avr/libraries/HID/src/HID.h
        )
generate_arduino_library(HID-Project
        BOARD leonardo
        SRCS  /usr/share/arduino-1.6.9/libraries/HID-Project/src/SingleReport/BootKeyboard.cpp
        HDRS /usr/share/arduino-1.6.9/libraries/HID-Project/src/HID-Project.h /usr/share/arduino-1.6.9/libraries/HID-Project/src/HID-Settings.h
        LIBS HID
        )

set(${CMAKE_PROJECT_NAME}_BOARD leonardo)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
set(${CMAKE_PROJECT_NAME}_LIBS HID-Project)

set(${CMAKE_PROJECT_NAME}_SKETCH keySender.ino)
include_directories(
        /usr/share/arduino-1.6.9/libraries/HID-Project/src
        /usr/share/arduino-1.6.9/hardware/arduino/avr/libraries/HID/src
)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
