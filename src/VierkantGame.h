
#ifndef PWS_VIERKANTGAME_H
#define PWS_VIERKANTGAME_H

#include "Game.h"

namespace pws {

    class VierkantGame : public Game {
    private:
        float x = 0;
        float y = 0;

        static constexpr int16_t GROOTTE = 16;
        static constexpr float SNELHEID = 120.0f;

        int16_t schermBreedte = 0;
        int16_t schermHoogte = 0;

        int16_t vorigeX = 0;
        int16_t vorigeY = 0;

        bool eersteFrame = true;

    public:
        void begin(Scherm& scherm) override;

        void update(
            float deltaTime,
            const InputSystem& input
        ) override;

        void render(Scherm& scherm) override;
    };

}

#endif // PWS_VIERKANTGAME_H
