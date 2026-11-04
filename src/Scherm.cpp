//
// Created by luukd on 04/11/2026.
//

#include <Arduino.h>
#include "Scherm.h"

namespace pws {
    Scherm::Scherm() {
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);
        hoogte = tft.height();
        breedte = tft.width();
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
} // pws