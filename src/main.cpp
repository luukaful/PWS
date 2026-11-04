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
static constexpr uint32_t FRAME_INTERVAL_MS = 16; // Ongeveer 60 FPS

uint32_t laatsteFrame = 0;

uint32_t fpsTimer = 0;
uint32_t frameCounter = 0;

uint64_t transferTotaal = 0;
uint32_t transferAantal = 0;

void setup() {
    Serial.begin(115200);

    // Hardware initialiseren
    if (!scherm.begin()) {
        Serial.println("Fout bij het initialiseren van het scherm!");
        while (true) {
            delay(1000);
        }
    }

    inputSystem.begin();

    Serial.println("Console gestart!");

    scherm.getTFT().drawString("Hello, World!", 10, 10);
    delay(2000);

    // Game initialiseren
    actieveGame->begin(scherm);
    laatsteFrame = millis();
    fpsTimer = laatsteFrame;
}


void loop() {
    uint32_t huidigeTijd = millis();

    // Framerate beperken
    if (huidigeTijd - laatsteFrame < FRAME_INTERVAL_MS) {
        return;
    }

    // DeltaTime berekenen
    float deltaTijd =
        (huidigeTijd - laatsteFrame) / 1000.0f;

    laatsteFrame = huidigeTijd;

    // Input verwerken
    inputSystem.updateKnopStates();

    // Game logica bijwerken
    actieveGame->update(deltaTijd, inputSystem);

    // Frame beginnen
    scherm.beginFrame();

    // Game renderen
    actieveGame->render(scherm);

    // Framebuffer versturen en overdracht meten
    uint32_t transferStart = micros();

    bool verstuurd = scherm.endFrame();

    uint32_t transferDuur = micros() - transferStart;

    // Een volledig gameframe is verwerkt
    frameCounter++;

    // Alleen echte schermoverdrachten meten
    if (verstuurd) {
        transferTotaal += transferDuur;
        transferAantal++;
    }

    // Diagnostiek iedere seconde
    uint32_t nu = millis();
    uint32_t verstrekenTijd = nu - fpsTimer;

    if (verstrekenTijd >= 1000) {
        float fps =
            frameCounter * 1000.0f / verstrekenTijd;

        float gemiddeldeTransfer = transferAantal > 0
            ? static_cast<float>(transferTotaal) / transferAantal
            : 0.0f;

        Serial.printf(
            "FPS: %.1f | Transfers: %lu | Gem. transfer: %.1f us\n",
            fps,
            static_cast<unsigned long>(transferAantal),
            gemiddeldeTransfer
        );

        // Meetwaarden resetten
        frameCounter = 0;
        transferAantal = 0;
        transferTotaal = 0;
        fpsTimer = nu;
    }
}

