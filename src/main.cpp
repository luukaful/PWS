#include <Arduino.h>
#include <Scherm.h>

#include "InputSystem.h"

pws::Scherm scherm = pws::Scherm();
pws::InputSystem inputSystem = pws::InputSystem();

void setup() {
    Serial.begin(115200);
    int breedte = scherm.getBreedte();
    int hoogte = scherm.getHoogte();
    Serial.println("Breedte: " + String(breedte));
    Serial.println("Hoogte: " + String(hoogte));
}

void loop() {
    inputSystem.updateKnopStates();
    if (inputSystem.knopGedrukt(KNOP_D_LEFT)) {
        scherm.getTFT().fillScreen(TFT_RED);
        Serial.println("Knop D-LEFT is ingedrukt");
    }
}