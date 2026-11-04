//
// Created by luukd on 04/11/2026.
//

#include <algorithm>
#include <Arduino.h>
#include "Scherm.h"

namespace pws {
    bool Scherm::begin() {
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);

        breedte = tft.width();
        hoogte = tft.height();

        // Framebuffer configureren
        sprite.setColorDepth(8);

        // Geheugen toewijzen voor de sprite
        if (sprite.createSprite(breedte, hoogte) == nullptr) { // Geen geheugen beschikbaar
            Serial.println("Fout bij het maken van de sprite!");
            return false;
        }

        sprite.fillSprite(TFT_BLACK);

        return true;
    }

    void Scherm::beginFrame() {
        // Begin een nieuw frame door de sprite te wissen
        sprite.fillSprite(TFT_BLACK);
        dirty = false;
    }

    void Scherm::markDirty(int16_t x, int16_t y, int16_t w, int16_t h) {
        if (w <= 0 || h <= 0) {
            return; // Ongeldige afmetingen
        }

        int16_t x0 = std::max<int16_t>(0, x);
        int16_t y0 = std::max<int16_t>(0, y);
        int16_t x1 = std::min<int16_t>(breedte, x + w);
        int16_t y1 = std::min<int16_t>(hoogte, y + h);

        if (x0 >= x1 || y0 >= y1) {
            return; // Geen overlap met het scherm
        }

        if (!dirty) {
            dirtyX0 = x0;
            dirtyY0 = y0;
            dirtyX1 = x1;
            dirtyY1 = y1;
            dirty = true;
        } else {
            dirtyX0 = std::min(dirtyX0, x0);
            dirtyY0 = std::min(dirtyY0, y0);
            dirtyX1 = std::max(dirtyX1, x1);
            dirtyY1 = std::max(dirtyY1, y1);
        }
    }

    bool Scherm::endFrame() {
        if (!dirty) {
            return false;
        }

        return sprite.pushSprite(
            dirtyX0, dirtyY0,
            dirtyX0, dirtyY0,
            dirtyX1 - dirtyX0,
            dirtyY1 - dirtyY0
        );
    }

    TFT_eSPI& Scherm::getTFT() {
        return tft;
    }

    TFT_eSprite& Scherm::getSprite() {
        return sprite;
    }

    int16_t Scherm::getBreedte() const {
        return breedte;
    }

    int16_t Scherm::getHoogte() const {
        return hoogte;
    }
}
