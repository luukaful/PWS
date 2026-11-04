//
// Created by luukd on 04/11/2026.
//

#include <Arduino.h>
#include "Scherm.h"

namespace pws {
    Scherm::Scherm() {
        tft.init();
        tft.fillScreen(TFT_BLACK);
        tft.setRotation(1);
        hoogte = tft.height();
        breedte = tft.width();
    }

    int Scherm::getBreedte() const {
        return breedte;
    }

    int Scherm::getHoogte() const {
        return hoogte;
    }
} // pws