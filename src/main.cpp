#include <Arduino.h>

#include "Scherm.h"
#include "InputSystem.h"

pws::Scherm scherm;
pws::InputSystem inputSystem;

void setup() {
    Serial.begin(115200);

    // Hardware initialiseren
    scherm.begin();
    inputSystem.begin();

    Serial.println("Console gestart!");

    Serial.println(
        "Breedte: " + String(scherm.getBreedte())
    );

    Serial.println(
        "Hoogte: " + String(scherm.getHoogte())
    );
}

void loop() {
    inputSystem.updateKnopStates();

    if (inputSystem.knopZojuistGedrukt(KNOP_D_UP)) {
        scherm.getTFT().fillScreen(TFT_RED);

        Serial.println("D-UP ingedrukt!");
    }

    if (inputSystem.knopZojuistGedrukt(KNOP_D_RIGHT)) {
        scherm.getTFT().fillScreen(TFT_BLUE);

        Serial.println("D-RIGHT ingedrukt!");
    }
}
