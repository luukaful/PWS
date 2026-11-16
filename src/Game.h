#ifndef PWS_GAME_H
#define PWS_GAME_H

#include "InputSystem.h"
#include "Scherm.h"

namespace pws {

    class Game {
    public:
        virtual ~Game() = default;

        // Reset de speltoestand bij elke start. Tekenen gebeurt alleen in render().
        virtual void begin(Scherm& scherm) = 0;

        // deltaTijd is de verstreken tijd in seconden sinds het vorige frame.
        virtual void update(float deltaTijd, const InputSystem& input) = 0;

        // Teken elk frame het volledige spelbeeld met raylib.
        virtual void render(Scherm& scherm) = 0;
    };

} // namespace pws

#endif // PWS_GAME_H
