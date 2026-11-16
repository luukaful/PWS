#include <Arduino.h>

#include "GameManager.h"
#include "InputSystem.h"
#include "Scherm.h"
#include "VierkantGame.h"

namespace {

    constexpr uint32_t FRAME_INTERVAL_MS = 16;
    constexpr uint32_t DIAGNOSTIEK_INTERVAL_MS = 1000;

    pws::Scherm scherm;
    pws::InputSystem inputSystem;
    pws::GameManager gameManager;
    pws::VierkantGame vierkantGame;

    uint32_t laatsteFrameMs = 0;
    uint32_t fpsTimerMs = 0;
    uint32_t frameAantal = 0;
    uint64_t transferTotaalUs = 0;
    uint32_t transferAantal = 0;

    void toonDiagnostiek(uint32_t nuMs) {
        const uint32_t verstrekenMs = nuMs - fpsTimerMs;
        if (verstrekenMs < DIAGNOSTIEK_INTERVAL_MS) {
            return;
        }

        // FPS telt gameframes; alleen gewijzigde beelden tellen als schermtransfer.
        const float fps = frameAantal * 1000.0f / verstrekenMs;
        const float gemiddeldeTransferUs = transferAantal > 0
            ? static_cast<float>(transferTotaalUs) / transferAantal
            : 0.0f;

        Serial.printf(
            "FPS: %.1f | Transfers: %lu | Transfer: %.1f us\n",
            fps,
            static_cast<unsigned long>(transferAantal),
            gemiddeldeTransferUs
        );

        frameAantal = 0;
        transferTotaalUs = 0;
        transferAantal = 0;
        fpsTimerMs = nuMs;
    }

} // namespace

void setup() {
    Serial.begin(115200);

    if (!scherm.begin()) {
        Serial.println("Scherminitialisatie mislukt!");
        while (true) {
            delay(1000);
        }
    }

    inputSystem.begin();
    gameManager.voegGameToe("Vierkant", vierkantGame);
    gameManager.begin();

    Serial.println("Console gestart!");

    laatsteFrameMs = millis();
    fpsTimerMs = laatsteFrameMs;
}

void loop() {
    const uint32_t nuMs = millis();
    // Unsigned aftrekken houdt de tijdmeting geldig wanneer millis() rondloopt.
    const uint32_t verstrekenMs = nuMs - laatsteFrameMs;
    if (verstrekenMs < FRAME_INTERVAL_MS) {
        return;
    }

    const float deltaTijd = verstrekenMs / 1000.0f;
    laatsteFrameMs = nuMs;

    inputSystem.updateKnopStates();

    // Reset de wijzigingen vóór update(): bij een spelwissel kan begin() al tekenen.
    scherm.beginFrame();
    gameManager.update(deltaTijd, inputSystem, scherm);
    gameManager.render(scherm);

    const uint32_t transferStartUs = micros();
    const bool verstuurd = scherm.endFrame();
    const uint32_t transferDuurUs = micros() - transferStartUs;

    ++frameAantal;
    if (verstuurd) {
        transferTotaalUs += transferDuurUs;
        ++transferAantal;
    }

    toonDiagnostiek(millis());
}
