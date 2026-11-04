//
// Created by luukd on 04/11/2026.
//

#include <Arduino.h>
#include "Scherm.h"

namespace pws {
    void Scherm::begin() {
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);

        breedte = tft.width();
        hoogte = tft.height();
    }

    TFT_eSPI& Scherm::getTFT() {
        return tft;
    }

    int16_t Scherm::getBreedte() const {
        return breedte;
    }

    int16_t Scherm::getHoogte() const {
        return hoogte;
    }
}
