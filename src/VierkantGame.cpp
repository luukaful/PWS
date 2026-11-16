#include "VierkantGame.h"

namespace pws {

    void VierkantGame::begin(Scherm& scherm) {
        schermBreedte = scherm.getBreedte();
        schermHoogte = scherm.getHoogte();

        x = static_cast<float>(schermBreedte - GROOTTE) / 2.0f;
        y = static_cast<float>(schermHoogte - GROOTTE) / 2.0f;
        vorigeX = static_cast<int16_t>(x);
        vorigeY = static_cast<int16_t>(y);
        eersteFrame = true;
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

    void VierkantGame::render(Scherm& scherm) {
        const auto pixelX = static_cast<int16_t>(x);
        const auto pixelY = static_cast<int16_t>(y);

        // De sprite bewaart het vorige beeld; zonder zichtbare beweging is tekenen overbodig.
        if (!eersteFrame && pixelX == vorigeX && pixelY == vorigeY) {
            return;
        }

        TFT_eSprite& sprite = scherm.getSprite();

        // Dit spel heeft een zwarte achtergrond. Bij een afbeelding moet je hier
        // het oorspronkelijke stukje achtergrond herstellen.
        if (!eersteFrame) {
            sprite.fillRect(vorigeX, vorigeY, GROOTTE, GROOTTE, TFT_BLACK);
            scherm.markDirty(vorigeX, vorigeY, GROOTTE, GROOTTE);
        }

        // Eerst wissen, dan tekenen: zo blijft het vierkant ook bij overlap intact.
        sprite.fillRect(pixelX, pixelY, GROOTTE, GROOTTE, TFT_GREEN);
        scherm.markDirty(pixelX, pixelY, GROOTTE, GROOTTE);

        vorigeX = pixelX;
        vorigeY = pixelY;
        eersteFrame = false;
    }

} // namespace pws
