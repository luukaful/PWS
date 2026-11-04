
#include <Arduino.h>

#include "Scherm.h"
#include "InputSystem.h"
#include "GameManager.h"
#include "VierkantGame.h"

// Hardware
pws::Scherm scherm;
pws::InputSystem inputSystem;

// GameManager
pws::GameManager gameManager;

// Games
pws::VierkantGame vierkantGame;

// Frame timing
static constexpr uint32_t FRAME_INTERVAL_MS = 16;

uint32_t laatsteFrame = 0;

// Diagnostiek
uint32_t fpsTimer = 0;
uint32_t frameCounter = 0;

uint64_t transferTotaal = 0;
uint32_t transferAantal = 0;

void setup() {
    Serial.begin(115200);

    // Hardware initialiseren
    if (!scherm.begin()) {
        Serial.println("Scherminitialisatie mislukt!");

        while (true) {
            delay(1000);
        }
    }

    inputSystem.begin();

    // Games registreren
    gameManager.voegGameToe("Vierkant", vierkantGame);

    // Hoofdmenu starten
    gameManager.begin();

    Serial.println("Console gestart!");

    laatsteFrame = millis();
    fpsTimer = laatsteFrame;
}

void loop() {
    uint32_t huidigeTijd = millis();

    // Framerate beperken
    if (huidigeTijd - laatsteFrame < FRAME_INTERVAL_MS) {
        return;
    }

    // DeltaTime
    float deltaTijd =
        (huidigeTijd - laatsteFrame) / 1000.0f;

    laatsteFrame = huidigeTijd;

    // Knoppen bijwerken
    inputSystem.updateKnopStates();

    // GameManager bijwerken
    gameManager.update(deltaTijd, inputSystem, scherm);

    // Frame beginnen
    scherm.beginFrame();

    // GameManager tekent menu of actieve game
    gameManager.render(scherm);

    // Frame versturen
    uint32_t start = micros();

    bool verstuurd = scherm.endFrame();

    uint32_t duur = micros() - start;

    frameCounter++;

    if (verstuurd) {
        transferTotaal += duur;
        transferAantal++;
    }

    // Diagnostiek
    uint32_t nu = millis();

    if (nu - fpsTimer >= 1000) {
        uint32_t verstreken = nu - fpsTimer;

        float fps =
            frameCounter * 1000.0f / verstreken;

        float gemiddeldeTransfer =
            transferAantal > 0
            ? static_cast<float>(transferTotaal)
              / transferAantal
            : 0.0f;

        Serial.printf(
            "FPS: %.1f | Transfers: %lu | Transfer: %.1f us\n",
            fps,
            static_cast<unsigned long>(transferAantal),
            gemiddeldeTransfer
        );

        frameCounter = 0;
        transferTotaal = 0;
        transferAantal = 0;
        fpsTimer = nu;
    }
}
