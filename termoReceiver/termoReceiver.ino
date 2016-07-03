#include <Arduino.h>
#include "RF24.h"

struct response {
    long temp;     //*100
    long pressure; //*100
    long altitude; //*100
    long signalStrength;
} sensorReadings;
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup(void) {
    Serial.begin(9600);
    radio.begin();
    radio.enableAckPayload();
    radio.openWritingPipe(pipe);
}

void loop(void) {
    radio.write(&sensorReadings, sizeof(sensorReadings));
    if (radio.isAckPayloadAvailable()) {
        radio.read(&sensorReadings, sizeof(sensorReadings));
        Serial.print("Temp: ");
        Serial.print(sensorReadings.temp / 100.0);
        Serial.print(". Pressure: ");
        Serial.print(sensorReadings.pressure / 100.0);
        Serial.print(". Altitude: ");
        Serial.println(sensorReadings.altitude / 100.0);
    } else {
        Serial.println("No ack");
    }
    delay(1000);
}