#include "Scherm.h"
#include "RaylibTftPort.h"

#include <Arduino.h>

namespace pws {

    bool Scherm::begin() {
        // Deze ESP32-port gebruikt PSRAM voor raylib-geheugenallocaties.
        if (!psramFound()) {
            Serial.println("Geen PSRAM gevonden. Controleer de module en het PSRAM-profiel.");
            return false;
        }

        Serial.printf("PSRAM: %lu bytes; vrij: %lu bytes\n",
            static_cast<unsigned long>(ESP.getPsramSize()),
            static_cast<unsigned long>(ESP.getFreePsram()));

        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);
        breedte = tft.width();
        hoogte = tft.height();

        if (!verbindRaylibScherm(tft)) {
            Serial.println("Kon de raylib-schermkoppeling niet initialiseren.");
            return false;
        }

        // Op deze port maakt InitWindow() framebuffers aan voor het fysieke TFT.
        SetTraceLogLevel(LOG_WARNING);
        InitWindow(breedte, hoogte, "PWS Console");
        if (!IsWindowReady()) {
            return false;
        }

        // main.cpp bepaalt het tempo. Zo wachten we niet tweemaal per frame.
        SetTargetFPS(0);
        return true;
    }

    void Scherm::beginFrame(Color achtergrond) {
        BeginDrawing();
        ClearBackground(achtergrond);
    }

    void Scherm::endFrame() {
        // Verwerkt de tekenopdrachten en verstuurt het beeld via RaylibTftPort.
        EndDrawing();
    }

    int16_t Scherm::getBreedte() const {
        return breedte;
    }

    int16_t Scherm::getHoogte() const {
        return hoogte;
    }

} // namespace pws
