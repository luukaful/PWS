#include "VierkantGame.h"

namespace pws {

    void VierkantGame::begin(Scherm& scherm) {
        schermBreedte = scherm.getBreedte();
        schermHoogte = scherm.getHoogte();

        x = static_cast<float>(schermBreedte - GROOTTE) / 2.0f;
        y = static_cast<float>(schermHoogte - GROOTTE) / 2.0f;
    }

    void VierkantGame::update(float deltaTijd, const InputSystem& input) {
        const float beweging = SNELHEID * deltaTijd;

        if (input.knopGedrukt(Knop::D_LEFT)) {
            x -= beweging;
        }
        if (input.knopGedrukt(Knop::D_RIGHT)) {
            x += beweging;
        }
        if (input.knopGedrukt(Knop::D_UP)) {
            y -= beweging;
        }
        if (input.knopGedrukt(Knop::D_DOWN)) {
            y += beweging;
        }

        // x en y zijn de linkerbovenhoek; het hele vierkant moet binnen beeld blijven.
        const float maxX = static_cast<float>(schermBreedte) - GROOTTE;
        const float maxY = static_cast<float>(schermHoogte) - GROOTTE;

        if (x < 0.0f) {
            x = 0.0f;
        }
        if (y < 0.0f) {
            y = 0.0f;
        }
        if (x > maxX) {
            x = maxX;
        }
        if (y > maxY) {
            y = maxY;
        }
    }

    void VierkantGame::render(Scherm&) {
        // De achtergrond is aan het begin van dit frame al gewist.
        DrawRectangle(static_cast<int>(x), static_cast<int>(y), GROOTTE, GROOTTE, GREEN);
    }

} // namespace pws
