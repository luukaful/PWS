#include "GameManager.h"

namespace pws {

    bool GameManager::voegGameToe(const char* naam, Game& game) {
        if (aantalGames >= MAX_GAMES || naam == nullptr) {
            return false;
        }

        GameEntry& entry = games[aantalGames];
        entry.naam = naam;
        entry.game = &game;
        ++aantalGames;
        return true;
    }

    void GameManager::begin() {
        actieveGame = nullptr;
        geselecteerdeGame = 0;
    }

    void GameManager::update(float deltaTijd, const InputSystem& input, Scherm& scherm) {
        if (actieveGame == nullptr) {
            updateMenu(input, scherm);
            return;
        }

        if (input.knopZojuistGedrukt(Knop::START)) {
            actieveGame = nullptr;
            return;
        }

        actieveGame->update(deltaTijd, input);
    }

    void GameManager::updateMenu(const InputSystem& input, Scherm& scherm) {
        if (aantalGames == 0) {
            return;
        }

        // Modulo laat de selectie doorlopen van het laatste naar het eerste spel.
        if (input.knopZojuistGedrukt(Knop::D_DOWN)) {
            geselecteerdeGame = (geselecteerdeGame + 1) % aantalGames;
        } else if (input.knopZojuistGedrukt(Knop::D_UP)) {
            geselecteerdeGame = (geselecteerdeGame + aantalGames - 1) % aantalGames;
        }

        if (input.knopZojuistGedrukt(Knop::A)) {
            actieveGame = games[geselecteerdeGame].game;

            actieveGame->begin(scherm);
        }
    }

    void GameManager::render(Scherm& scherm) {
        if (actieveGame != nullptr) {
            actieveGame->render(scherm);
        } else {
            // Ook het stilstaande menu moet na ClearBackground() opnieuw worden getekend.
            tekenMenu(scherm);
        }
    }

    void GameManager::tekenMenu(Scherm& scherm) {
        constexpr int16_t RIJ_START_Y = 69;
        constexpr int16_t RIJ_AFSTAND = 28;
        constexpr int16_t RIJ_HOOGTE = 24;

        const int16_t breedte = scherm.getBreedte();
        const int16_t hoogte = scherm.getHoogte();

        DrawText("PWS CONSOLE", 14, 10, 24, WHITE);

        DrawText("Selecteer een spel:", 14, 46, 16, LIGHTGRAY);

        if (aantalGames == 0) {
            DrawText("Geen spellen gevonden", 14, 80, 16, LIGHTGRAY);
        }

        for (uint8_t i = 0; i < aantalGames; ++i) {
            const int16_t y = RIJ_START_Y + i * RIJ_AFSTAND;
            const bool geselecteerd = i == geselecteerdeGame;
            const Color achtergrond = geselecteerd ? BLUE : DARKGRAY;

            DrawRectangle(12, y, breedte - 24, RIJ_HOOGTE, achtergrond);
            DrawText(games[i].naam, 24, y + 4, 16, WHITE);
        }

        DrawText("A: Start   START: Terug", 14, hoogte - 22, 12, LIGHTGRAY);
    }

} // namespace pws
