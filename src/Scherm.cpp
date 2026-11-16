#include "Scherm.h"

#include <algorithm>
#include <Arduino.h>

namespace pws {

    bool Scherm::begin() {
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);

        breedte = tft.width();
        hoogte = tft.height();

        // Acht bits per pixel beperkt het geheugengebruik van de volledige framebuffer.
        sprite.setColorDepth(8);
        if (sprite.createSprite(breedte, hoogte) == nullptr) {
            Serial.println("Fout bij het maken van de sprite!");
            return false;
        }

        sprite.fillSprite(TFT_BLACK);
        return true;
    }

    void Scherm::beginFrame() {
        dirty = false;
    }

    void Scherm::clear(uint16_t kleur) {
        sprite.fillSprite(kleur);
        markDirty(0, 0, breedte, hoogte);
    }

    void Scherm::markDirty(int16_t x, int16_t y, int16_t gebiedBreedte, int16_t gebiedHoogte) {
        if (gebiedBreedte <= 0 || gebiedHoogte <= 0) {
            return;
        }

        // Knip het gebied af op de schermranden voordat het wordt samengevoegd.
        const int16_t x0 = std::max<int16_t>(0, x);
        const int16_t y0 = std::max<int16_t>(0, y);
        const int16_t x1 = std::min<int16_t>(breedte, x + gebiedBreedte);
        const int16_t y1 = std::min<int16_t>(hoogte, y + gebiedHoogte);

        if (x0 >= x1 || y0 >= y1) {
            return;
        }

        if (!dirty) {
            dirtyX0 = x0;
            dirtyY0 = y0;
            dirtyX1 = x1;
            dirtyY1 = y1;
            dirty = true;
        } else {
            // Alle wijzigingen gaan samen in één omvattende rechthoek naar het scherm.
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

        // Het eerste paar is de bestemming op het TFT, het tweede de bron in de sprite.
        return sprite.pushSprite(
            dirtyX0, dirtyY0,
            dirtyX0, dirtyY0,
            dirtyX1 - dirtyX0, dirtyY1 - dirtyY0
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

} // namespace pws
