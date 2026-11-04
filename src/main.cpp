#include <Arduino.h>
#include <Scherm.h>

void setup() {
    Serial.begin(115200);
    const pws::Scherm scherm = pws::Scherm();
    int breedte = scherm.getBreedte();
    int hoogte = scherm.getHoogte();
    Serial.println("Breedte: " + String(breedte));
    Serial.println("Hoogte: " + String(hoogte));
}

void loop() {
    // put your main code here, to run repeatedly:
}