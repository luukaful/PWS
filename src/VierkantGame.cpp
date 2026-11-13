
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

        if (x > static_cast<float>(schermBreedte) - GROOTTE) {
            x = static_cast<float>(schermBreedte) - GROOTTE;
        }

        if (y > static_cast<float>(schermHoogte) - GROOTTE) {
            y = static_cast<float>(schermHoogte) - GROOTTE;
        }
    }



    void VierkantGame::render(Scherm& scherm) {
        TFT_eSprite& sprite = scherm.getSprite();

        auto nx = static_cast<int16_t>(x);
        auto ny = static_cast<int16_t>(y);

        // De vorige pixels blijven bewaard: bij stilstand is niets nodig.
        if (!eersteFrame &&
            nx == vorigeX &&
            ny == vorigeY) {
            return;
        }

        // Herstel eerst de zwarte achtergrond op de oude positie.
        // Bij een getekende achtergrond moet hier die achtergrond terugkomen.
        if (!eersteFrame) {
            sprite.fillRect(
                vorigeX, vorigeY, GROOTTE, GROOTTE, TFT_BLACK
            );
            scherm.markDirty(vorigeX, vorigeY, GROOTTE, GROOTTE);
        }

        // Teken daarna de nieuwe positie, ook als beide posities overlappen.
        sprite.fillRect(
            nx, ny, GROOTTE, GROOTTE, TFT_GREEN
        );
        // Scherm voegt deze rechthoek samen met de oude positie.
        scherm.markDirty(nx, ny, GROOTTE, GROOTTE);

        vorigeX = nx;
        vorigeY = ny;
        eersteFrame = false;
    }



}
