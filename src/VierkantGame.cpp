
#include "VierkantGame.h"

namespace pws {

    void VierkantGame::begin(Scherm& scherm) {
        schermBreedte = scherm.getBreedte();
        schermHoogte = scherm.getHoogte();

        // Begin in het midden
        x = (schermBreedte - GROOTTE) / 2.0f;
        y = (schermHoogte - GROOTTE) / 2.0f;

        eersteFrame = true;

        scherm.getTFT().fillScreen(TFT_BLACK);
    }

    void VierkantGame::update(
        float deltaTime,
        const InputSystem& input
    ) {
        float beweging = SNELHEID * deltaTime;

        if (input.knopGedrukt(KNOP_D_LEFT)) {
            x -= beweging;
        }

        if (input.knopGedrukt(KNOP_D_RIGHT)) {
            x += beweging;
        }

        if (input.knopGedrukt(KNOP_D_UP)) {
            y -= beweging;
        }

        if (input.knopGedrukt(KNOP_D_DOWN)) {
            y += beweging;
        }

        // Voorkom dat de speler buiten beeld gaat
        if (x < 0) x = 0;
        if (y < 0) y = 0;

        if (x > schermBreedte - GROOTTE) {
            x = schermBreedte - GROOTTE;
        }

        if (y > schermHoogte - GROOTTE) {
            y = schermHoogte - GROOTTE;
        }
    }

    void VierkantGame::render(Scherm& scherm) {
        TFT_eSPI& tft = scherm.getTFT();

        int16_t nieuweX = static_cast<int16_t>(x);
        int16_t nieuweY = static_cast<int16_t>(y);

        // Niets veranderd? Dan hoeven we niet te tekenen
        if (!eersteFrame &&
            nieuweX == vorigeX &&
            nieuweY == vorigeY) {
            return;
            }

        // Oude positie wissen
        if (!eersteFrame) {
            tft.fillRect(
                vorigeX, vorigeY,
                GROOTTE, GROOTTE,
                TFT_BLACK
            );
        }

        // Nieuwe positie tekenen
        tft.fillRect(
            nieuweX, nieuweY,
            GROOTTE, GROOTTE,
            TFT_GREEN
        );

        vorigeX = nieuweX;
        vorigeY = nieuweY;
        eersteFrame = false;
    }

}
