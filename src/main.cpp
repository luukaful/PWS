#include <Arduino.h>

#include "GameManager.h"
#include "InputSystem.h"
#include "Scherm.h"
#include "RaylibTftPort.h"
#include "VierkantGame.h"

namespace {

    constexpr uint32_t FRAME_INTERVAL_MS = 16; // Bovengrens van ongeveer 30 FPS.
    constexpr uint32_t DIAGNOSTIEK_INTERVAL_MS = 1000;

    pws::Scherm scherm;
    pws::InputSystem inputSystem;
    pws::GameManager gameManager;
    pws::VierkantGame vierkantGame;

    uint32_t laatsteFrameMs = 0;
    uint32_t fpsTimerMs = 0;
    uint32_t frameAantal = 0;
    uint64_t tekenTotaalUs = 0;
    uint64_t vergelijkTotaalUs = 0;
    uint64_t verstuurTotaalUs = 0;
    uint64_t pixelTotaal = 0;
    uint32_t transfers = 0;

    void toonDiagnostiek(uint32_t nuMs) {
        const uint32_t verstrekenMs = nuMs - fpsTimerMs;
        if (verstrekenMs < DIAGNOSTIEK_INTERVAL_MS) {
            return;
        }

        const float fps = frameAantal * 1000.0f / verstrekenMs;
        const float tekenMs = frameAantal > 0
            ? static_cast<float>(tekenTotaalUs) / frameAantal / 1000.0f
            : 0.0f;

        const float deler = frameAantal > 0 ? static_cast<float>(frameAantal) : 1.0f;
        // Alle tijden zijn gemiddelden per gameframe, ook frames zonder transfer.
        Serial.printf(
            "FPS: %.1f | Framewerk: %.2f ms | Vergelijk: %.2f ms | "
            "TFT: %.2f ms | Pixels/frame: %.0f | Transfers: %lu | Vrij PSRAM: %lu\n",
            fps, tekenMs, vergelijkTotaalUs / deler / 1000.0f,
            verstuurTotaalUs / deler / 1000.0f, pixelTotaal / deler,
            static_cast<unsigned long>(transfers),
            static_cast<unsigned long>(ESP.getFreePsram()));

        frameAantal = 0;
        tekenTotaalUs = 0;
        vergelijkTotaalUs = 0;
        verstuurTotaalUs = 0;
        pixelTotaal = 0;
        transfers = 0;
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

    Serial.println("Console met raylib gestart!");
    laatsteFrameMs = millis();
    fpsTimerMs = laatsteFrameMs;
}

void loop() {
    const uint32_t nuMs = millis();
    const uint32_t verstrekenMs = nuMs - laatsteFrameMs;
    if (verstrekenMs < FRAME_INTERVAL_MS) {
        delay(1);
        return;
    }
    laatsteFrameMs = nuMs;

    // Een lange hapering mag de speler niet ineens over het hele scherm sturen.
    const float deltaTijd = (verstrekenMs > 100 ? 100 : verstrekenMs) / 1000.0f;
    inputSystem.updateKnopStates();
    gameManager.update(deltaTijd, inputSystem, scherm);

    const uint32_t tekenStartUs = micros();
    scherm.beginFrame();
    gameManager.render(scherm);
    scherm.endFrame();
    tekenTotaalUs += micros() - tekenStartUs;
    const pws::SchermTransferMeting meting = pws::laatsteSchermTransfer();
    vergelijkTotaalUs += meting.vergelijkenUs;
    verstuurTotaalUs += meting.versturenUs;
    pixelTotaal += meting.verstuurdePixels;
    if (meting.verstuurdePixels > 0) ++transfers;
    ++frameAantal;

    toonDiagnostiek(millis());
}
