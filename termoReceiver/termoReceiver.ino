#include <Arduino.h>
#include "nRF24L01.h"
#include "RF24.h"

int msg[2];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void) {
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
}

void loop(void) {
    if (radio.available()) {
        bool done = false;
        while (!done) {
            done = radio.read(msg, 4);
            int Fract = msg[0] % 100;
            Serial.print(msg[0] / 100);

            Serial.print(".");
            if (Fract < 10) Serial.print("0");
            Serial.print(Fract);
            delay(10);
        }
    }
    else {
        delay(100);
        Serial.println("No radio available");
    }
}