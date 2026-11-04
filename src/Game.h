
#ifndef PWS_GAME_H
#define PWS_GAME_H

#include "Scherm.h"
#include "InputSystem.h"

namespace pws {
    class Game {
    public:
        virtual ~Game() = default;

        virtual void begin(Scherm& scherm) = 0;

        virtual void update(float deltaTijd, const InputSystem& input) = 0;

        virtual void render(Scherm& scherm) = 0;
    };
}

#endif // PWS_GAME_H
