
#include <algorithm>
#include "VierkantGame.h"

namespace pws {


    void VierkantGame::begin(Scherm& scherm) {
        schermBreedte = scherm.getBreedte();
        schermHoogte = scherm.getHoogte();

        x = (schermBreedte - GROOTTE) / 2.0f;
        y = (schermHoogte - GROOTTE) / 2.0f;

        vorigeX = static_cast<int16_t>(x);
        vorigeY = static_cast<int16_t>(y);

        eersteFrame = true;
    }


    void VierkantGame::update(
        float deltaTime,
        const InputSystem& input
    ) {
        float beweging = SNELHEID * deltaTime;

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
        TFT_eSprite& sprite = scherm.getSprite();

        int16_t nx = static_cast<int16_t>(x);
        int16_t ny = static_cast<int16_t>(y);

        // Geen beweging: geen nieuwe schermupdate
        if (!eersteFrame &&
            nx == vorigeX &&
            ny == vorigeY) {
            return;
            }

        // Teken op de framebuffer
        sprite.fillRect(
            nx, ny, GROOTTE, GROOTTE, TFT_GREEN
        );

        // Bepaal het gewijzigde gebied
        int16_t x0 = eersteFrame
            ? nx : std::min(nx, vorigeX);

        int16_t y0 = eersteFrame
            ? ny : std::min(ny, vorigeY);

        int16_t x1 = eersteFrame
            ? nx + GROOTTE
            : std::max(nx, vorigeX) + GROOTTE;

        int16_t y1 = eersteFrame
            ? ny + GROOTTE
            : std::max(ny, vorigeY) + GROOTTE;

        scherm.markDirty(
            x0, y0,
            x1 - x0,
            y1 - y0
        );

        vorigeX = nx;
        vorigeY = ny;
        eersteFrame = false;
    }



}
