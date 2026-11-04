#include <Arduino.h>

#include "Scherm.h"
#include "InputSystem.h"
#include "VierkantGame.h"

// Hardware
pws::Scherm scherm;
pws::InputSystem inputSystem;

// Games
pws::VierkantGame vierkantGame;

// Actieve game
pws::Game* actieveGame = &vierkantGame;

// Frame timing
static constexpr uint32_t FRAME_INTERVAL_MS = 16; // ~60 FPS
uint32_t laatsteFrame = 0;

void setup() {
    Serial.begin(115200);

    // Hardware initialiseren
    scherm.begin();
    inputSystem.begin();

    Serial.println("Console gestart!");

    // Game initialiseren
    actieveGame->begin(scherm);

    laatsteFrame = millis();
}

void loop() {
    uint32_t huidigeTijd = millis();

    // Ongeveer 60 FPS
    if (huidigeTijd - laatsteFrame < FRAME_INTERVAL_MS) {
        return;
    }

    float deltaTijd = (huidigeTijd - laatsteFrame) / 1000.0f;
    laatsteFrame = huidigeTijd;

    // Knopstatus bijwerken
    inputSystem.updateKnopStates();

    // Game logica bijwerken
    actieveGame->update(deltaTijd, inputSystem);

    // Game renderen
    actieveGame->render(scherm);
}
