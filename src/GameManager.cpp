
#include "GameManager.h"

namespace pws {

    bool GameManager::voegGameToe(
        const char* naam,
        Game& game
    ) {
        if (aantalGames >= MAX_GAMES || naam == nullptr) {
            return false;
        }

        games[aantalGames].naam = naam;
        games[aantalGames].game = &game;

        aantalGames++;

        return true;
    }

    void GameManager::begin() {
        actieveGame = nullptr;
        geselecteerdeGame = 0;
        volledigVernieuwen = true;
    }

    void GameManager::update(
        float deltaTijd,
        const InputSystem& input,
        Scherm& scherm
    ) {
        // Hoofdmenu
        if (actieveGame == nullptr) {
            if (aantalGames == 0) {
                return;
            }

            if (input.knopZojuistGedrukt(KNOP_D_DOWN)) {
                geselecteerdeGame =
                    (geselecteerdeGame + 1) % aantalGames;

                volledigVernieuwen = true;
            }
            else if (input.knopZojuistGedrukt(KNOP_D_UP)) {
                geselecteerdeGame =
                    (geselecteerdeGame + aantalGames - 1)
                    % aantalGames;

                volledigVernieuwen = true;
            }

            // Geselecteerde game starten
            if (input.knopZojuistGedrukt(KNOP_A)) {
                actieveGame = games[geselecteerdeGame].game;

                actieveGame->begin(scherm);
                volledigVernieuwen = true;
            }

            return;
        }

        // Met B terug naar het hoofdmenu
        if (input.knopZojuistGedrukt(KNOP_START)) {
            actieveGame = nullptr;
            volledigVernieuwen = true;
            return;
        }

        // Actieve game bijwerken
        actieveGame->update(deltaTijd, input);
    }

    void GameManager::render(Scherm& scherm) {
        if (actieveGame == nullptr) {
            // Menu alleen tekenen als het veranderd is
            if (volledigVernieuwen) {
                tekenMenu(scherm);
            }
        } else {
            actieveGame->render(scherm);
        }

        // Bij een gamewissel het hele scherm verversen
        if (volledigVernieuwen) {
            scherm.markDirty(
                0, 0,
                scherm.getBreedte(),
                scherm.getHoogte()
            );

            volledigVernieuwen = false;
        }
    }

    void GameManager::tekenMenu(Scherm& scherm) {
        TFT_eSprite& sprite = scherm.getSprite();

        int16_t breedte = scherm.getBreedte();
        int16_t hoogte = scherm.getHoogte();

        // Titel
        sprite.setTextColor(TFT_WHITE);
        sprite.drawString("PWS CONSOLE", 14, 10, 4);

        sprite.setTextColor(TFT_LIGHTGREY);
        sprite.drawString("Selecteer een spel:", 14, 46, 2);

        if (aantalGames == 0) {
            sprite.drawString(
                "Geen spellen gevonden",
                14, 80, 2
            );
        }

        // Geregistreerde games tekenen
        for (int i = 0; i < aantalGames; i++) {
            int16_t y = 69 + i * 28;

            bool geselecteerd = i == geselecteerdeGame;

            uint16_t achtergrond = geselecteerd
                ? TFT_BLUE
                : TFT_DARKGREY;

            sprite.fillRoundRect(
                12, y,
                breedte - 24, 24,
                4,
                achtergrond
            );

            sprite.setTextColor(TFT_WHITE, achtergrond);

            sprite.drawString(
                games[i].naam,
                24, y + 3, 2
            );
        }

        // Bedieningsinstructies
        sprite.setTextColor(TFT_LIGHTGREY);

        sprite.drawString(
            "A: Start   B: Terug",
            14, hoogte - 22, 2
        );
    }

}
