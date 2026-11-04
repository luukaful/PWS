
#ifndef PWS_GAMEMANAGER_H
#define PWS_GAMEMANAGER_H

#include "Game.h"

namespace pws {

    class GameManager {
    private:
        static constexpr uint8_t MAX_GAMES = 5;

        struct GameEntry {
            const char* naam = nullptr;
            Game* game = nullptr;
        };

        GameEntry games[MAX_GAMES] = {};

        uint8_t aantalGames = 0;
        uint8_t geselecteerdeGame = 0;

        // nullptr betekent dat het menu actief is
        Game* actieveGame = nullptr;

        // Bij een schermwissel alles opnieuw tekenen
        bool volledigVernieuwen = true;

        void tekenMenu(Scherm& scherm);

    public:
        bool voegGameToe(const char* naam, Game& game);

        void begin();

        void update(
            float deltaTijd,
            const InputSystem& input,
            Scherm& scherm
        );

        void render(Scherm& scherm);
    };

}

#endif // PWS_GAMEMANAGER_H
