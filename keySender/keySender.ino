#include <Arduino.h>
#include "HID-Project.h"

const int pinLed = LED_BUILTIN;

void setup() {
    pinMode(pinLed, OUTPUT);
    BootKeyboard.begin();
}


void loop() {
    if (BootKeyboard.getLeds() & LED_NUM_LOCK) {
        digitalWrite(pinLed, HIGH);
        BootKeyboard.print('N');
        delay(250);
    } else {
        digitalWrite(pinLed, LOW);
    }
}