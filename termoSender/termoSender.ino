#include <Arduino.h>
#include "RF24.h"
#include "HP03.h"

struct response {
    long temp;     //*100
    long pressure; //*100
    long altitude; //*100
    long signalStrength;
} sensorReadings;

RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void showReadings() {
    Serial.print("Temp ");
    Serial.print(HP03.Temperature / 10.0);
    sensorReadings.temp = HP03.Temperature * 10;
    Serial.print(" Pressure ");
    Serial.print(HP03.Pressure / 100.0);
    sensorReadings.pressure = HP03.Pressure;
    HP03.distanceUnits = METERS;
    Serial.print("   Altitude: Meters = ");
    Serial.println(HP03.getAltitude(HP03.Pressure) / 10);
    sensorReadings.altitude = HP03.getAltitude(HP03.Pressure) * 10;
}

void setup(void) {
    Serial.begin(9600);
    radio.begin();
    radio.enableAckPayload();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
    if (HP03.begin() == false)
        Serial.println("Error getting HP03 calibration, check sensor connection");
}

void loop(void) {
    if (HP03.update() == false)
        Serial.println("Error getting HP03 data, check sensor connection");
    else {
        showReadings();
    }
    if (radio.available()) {
        bool done = false;
        while (!done) {
            byte tmp;
            done = radio.read(&tmp, sizeof(tmp));
            Serial.print(tmp);
        }
        radio.writeAckPayload(1, &sensorReadings, sizeof(sensorReadings));
    }
    delay(1000 - 85);
}
