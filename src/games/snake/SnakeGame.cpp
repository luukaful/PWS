#include "SnakeGame.h"

#include <algorithm>
#include <cstdio>

namespace {

    constexpr Color BACKGROUND{0, 0, 0, 255};
    constexpr Color BORDER{40, 68, 76, 255};
    constexpr Color BODY{53, 195, 124, 255};
    constexpr Color HEAD{159, 245, 114, 255};
    constexpr Color FOOD{255, 103, 114, 255};
    constexpr Color TEXT{222, 238, 235, 255};
    constexpr Color MUTED{123, 153, 158, 255};

    uint32_t randomValue() {
        return static_cast<uint32_t>(GetRandomValue(0, 0x3fffffff));
    }

} // namespace

namespace pws {

    void SnakeGame::begin(Scherm& scherm) {
        cellSize = std::max(4, std::min(
            (scherm.getBreedte() - 16) / SnakeModel::MAX_COLUMNS,
            (scherm.getHoogte() - 52) / SnakeModel::MAX_ROWS));
        boardX = (scherm.getBreedte() - SnakeModel::MAX_COLUMNS * cellSize) / 2;
        boardY = 32 + (scherm.getHoogte() - 52 - SnakeModel::MAX_ROWS * cellSize) / 2;
        reset();
    }

    void SnakeGame::reset() {
        model.reset(randomValue());
        elapsed = 0.0f;
        ready = true;
        paused = false;
    }

    float SnakeGame::stepSeconds() const {
        return std::max(0.07f, 0.16f - model.score() * 0.004f);
    }

    void SnakeGame::update(float deltaTijd, const InputSystem& input) {
        if (model.status() != SnakeStatus::Playing) {
            if (input.knopZojuistGedrukt(Knop::A)) reset();
            return;
        }
        if (ready) {
            // De A-druk waarmee het menu dit spel opende, wordt niet hergebruikt.
            if (input.knopZojuistGedrukt(Knop::A)) ready = false;
            return;
        }
        if (input.knopZojuistGedrukt(Knop::B)) {
            paused = !paused;
            elapsed = 0.0f;
            return;
        }
        if (paused) return;

        if (input.knopZojuistGedrukt(Knop::D_UP)) model.queueDirection(SnakeDirection::Up);
        else if (input.knopZojuistGedrukt(Knop::D_RIGHT)) model.queueDirection(SnakeDirection::Right);
        else if (input.knopZojuistGedrukt(Knop::D_DOWN)) model.queueDirection(SnakeDirection::Down);
        else if (input.knopZojuistGedrukt(Knop::D_LEFT)) model.queueDirection(SnakeDirection::Left);

        // Bewegingssnelheid is onafhankelijk van het aantal getekende frames.
        elapsed += std::max(0.0f, std::min(deltaTijd, 0.25f));
        while (elapsed >= stepSeconds() && model.status() == SnakeStatus::Playing) {
            elapsed -= stepSeconds();
            model.step(randomValue());
            bestScore = std::max(bestScore, model.score());
        }
    }

    void SnakeGame::drawCentered(const char* text, int y, int size, Color color, int width) const {
        DrawText(text, (width - MeasureText(text, size)) / 2, y, size, color);
    }

    void SnakeGame::render(Scherm& scherm) {
        const int width = scherm.getBreedte();
        const int height = scherm.getHoogte();
        // Scherm::beginFrame() heeft de achtergrond al zwart gemaakt.
        DrawText("SNAKE", 12, 9, 20, HEAD);
        char scoreText[48];
        std::snprintf(scoreText, sizeof(scoreText), "SCORE %d   BEST %d", model.score(), bestScore);
        DrawText(scoreText, width - MeasureText(scoreText, 12) - 12, 13, 12, TEXT);

        const int boardWidth = model.columns() * cellSize;
        const int boardHeight = model.rows() * cellSize;
        DrawRectangle(boardX - 2, boardY - 2, boardWidth + 4, 2, BORDER);
        DrawRectangle(boardX - 2, boardY + boardHeight, boardWidth + 4, 2, BORDER);
        DrawRectangle(boardX - 2, boardY, 2, boardHeight, BORDER);
        DrawRectangle(boardX + boardWidth, boardY, 2, boardHeight, BORDER);

        if (model.status() != SnakeStatus::Won) {
            const SnakeCell food = model.food();
            DrawRectangle(boardX + food.x * cellSize + 2, boardY + food.y * cellSize + 2,
                          cellSize - 4, cellSize - 4, FOOD);
        }
        for (int i = model.length() - 1; i >= 0; --i) {
            const SnakeCell cell = model.segment(i);
            DrawRectangle(boardX + cell.x * cellSize + 1, boardY + cell.y * cellSize + 1,
                          cellSize - 2, cellSize - 2, i == 0 ? HEAD : BODY);
        }

        const SnakeCell head = model.segment(0);
        const int hx = boardX + head.x * cellSize;
        const int hy = boardY + head.y * cellSize;
        const int near = 3, far = cellSize - 5;
        switch (model.direction()) {
            case SnakeDirection::Right:
                DrawRectangle(hx + far, hy + near, 2, 2, BACKGROUND);
                DrawRectangle(hx + far, hy + far, 2, 2, BACKGROUND); break;
            case SnakeDirection::Left:
                DrawRectangle(hx + near, hy + near, 2, 2, BACKGROUND);
                DrawRectangle(hx + near, hy + far, 2, 2, BACKGROUND); break;
            case SnakeDirection::Up:
                DrawRectangle(hx + near, hy + near, 2, 2, BACKGROUND);
                DrawRectangle(hx + far, hy + near, 2, 2, BACKGROUND); break;
            case SnakeDirection::Down:
                DrawRectangle(hx + near, hy + far, 2, 2, BACKGROUND);
                DrawRectangle(hx + far, hy + far, 2, 2, BACKGROUND); break;
        }

        drawCentered("D-pad: sturen   B: pauze   START: menu", height - 15, 10, MUTED, width);

        if (ready || paused || model.status() != SnakeStatus::Playing) {
            const int panelWidth = std::min(240, width - 16);
            const int panelX = (width - panelWidth) / 2;
            const int panelY = boardY + boardHeight / 2 - 38;
            DrawRectangle(panelX - 1, panelY - 1, panelWidth + 2, 78, BORDER);
            DrawRectangle(panelX, panelY, panelWidth, 76, BACKGROUND);
            const char* title = ready ? "KLAAR?" : paused ? "PAUZE" :
                model.status() == SnakeStatus::Won ? "GEWONNEN!" : "GAME OVER";
            const char* hint = ready ? "A: starten" : paused ? "B: verder" : "A: opnieuw";
            drawCentered(title, panelY + 14, 24, HEAD, width);
            drawCentered(hint, panelY + 48, 14, TEXT, width);
        }
    }

} // namespace pws
