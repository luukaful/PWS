#ifndef PWS_VIERKANTGAME_H
#define PWS_VIERKANTGAME_H

#include "Game.h"

namespace pws {

    class VierkantGame : public Game {
    public:
        void begin(Scherm& scherm) override;
        void update(float deltaTijd, const InputSystem& input) override;
        void render(Scherm& scherm) override;

    private:
        static constexpr int16_t GROOTTE = 16;
        static constexpr float SNELHEID = 120.0f; // Pixels per seconde.

        // Bewaar fracties van pixels zodat kleine bewegingen niet verloren gaan.
        float x = 0.0f;
        float y = 0.0f;

        int16_t schermBreedte = 0;
        int16_t schermHoogte = 0;

        int16_t vorigeX = 0;
        int16_t vorigeY = 0;
        bool eersteFrame = true;
    };

} // namespace pws

#endif // PWS_VIERKANTGAME_H
