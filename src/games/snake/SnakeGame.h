#ifndef PWS_SNAKE_GAME_H
#define PWS_SNAKE_GAME_H

#include "../../Game.h"
#include "SnakeModel.h"

namespace pws {

    class SnakeGame : public Game {
    public:
        void begin(Scherm& scherm) override;
        void update(float deltaTijd, const InputSystem& input) override;
        void render(Scherm& scherm) override;

    private:
        SnakeModel model;
        int cellSize = 12;
        int boardX = 0;
        int boardY = 0;
        int bestScore = 0; // Bewaard tot de console opnieuw opstart.
        float elapsed = 0.0f;
        bool ready = true;
        bool paused = false;

        void reset();
        float stepSeconds() const;
        void drawCentered(const char* text, int y, int size, Color color, int width) const;
    };

} // namespace pws

#endif
