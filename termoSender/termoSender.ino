#include <Arduino.h>
#include "RF24.h"
#include "HP03.h"
#include "Wire.h"
#include <SPI.h>
#include <SD.h>

struct response {
    long temp;     //*100
    long pressure; //*100
    long altitude; //*100
    long signalStrength;
} sensorReadings;

#define DS3231_I2C_ADDRESS 0x68
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const int chipSelect = 4;

byte decToBcd(byte val) {
    return ((val / 10 * 16) + (val % 10));
}

byte bcdToDec(byte val) {
    return ((val / 16 * 10) + (val % 16));
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void displayTime() {
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

    Serial.print("Date: ");
    Serial.print(dayOfMonth, DEC);
    Serial.print('/');
    Serial.print(month, DEC);
    Serial.print('/');
    Serial.print(year, DEC);

    Serial.print(" Time: ");
    Serial.print(hour, DEC);
    Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second < 10) Serial.print("0");
    Serial.print(second, DEC);

    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print("Date: ");
        dataFile.print(dayOfMonth, DEC);
        dataFile.print('/');
        dataFile.print(month, DEC);
        dataFile.print('/');
        dataFile.print(year, DEC);

        dataFile.print(" Time: ");
        dataFile.print(hour, DEC);
        dataFile.print(":");
        if (minute < 10) dataFile.print("0");
        dataFile.print(minute, DEC);
        dataFile.print(":");
        if (second < 10) dataFile.print("0");
        dataFile.print(second, DEC);
        dataFile.close();
    } else {
        Serial.println("\nFailed to open datalog file.");
    }
}

void showReadings() {
    sensorReadings.temp = HP03.Temperature * 10;
    sensorReadings.pressure = HP03.Pressure;
    sensorReadings.altitude = HP03.getAltitude(HP03.Pressure) * 10;

    Serial.print(" Temp: ");
    Serial.print(HP03.Temperature / 10.0);
    Serial.print(" Pressure: ");
    Serial.print(HP03.Pressure / 100.0);
    Serial.print(" Altitude: ");
    Serial.println(HP03.getAltitude(HP03.Pressure) / 10);

    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print(" Temp: ");
        dataFile.print(HP03.Temperature / 10.0);
        dataFile.print(" Pressure: ");
        dataFile.print(HP03.Pressure / 100.0);
        dataFile.print(" Altitude: ");
        dataFile.println(HP03.getAltitude(HP03.Pressure) / 10);
        dataFile.close();
    } else {
        Serial.println("\nFailed to open datalog file.");
    }
}

Sd2Card card;

void setup(void) {
    Serial.begin(9600);

    if (!SD.begin(chipSelect)) {
        Serial.println("Failed to initialize SD card.");
    } else {
        Serial.println("SD card initialization succeed.");
    }
    radio.begin();
    radio.enableAckPayload();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
    if (HP03.begin()) {
        Serial.println("HP03 initialization succeed.");
        HP03.distanceUnits = METERS;
    } else {
        Serial.println("Failed to initialize HP03.");
    }

    Wire.begin();
    //setDS3231time(00,22,2,1,18,07,16);
}

void loop(void) {
    displayTime();
    if (HP03.update()) {
        showReadings();
    } else {
        Serial.println("Error getting HP03 data, check sensor connection");
    }
    if (radio.available()) {
        bool done = false;
        while (!done) {
            byte tmp;
            done = radio.read(&tmp, sizeof(tmp));
            //Serial.println(tmp);
        }
        radio.writeAckPayload(1, &sensorReadings, sizeof(sensorReadings));
    }
    delay(1000 - 85);
}
