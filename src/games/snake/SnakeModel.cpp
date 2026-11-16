#include "SnakeModel.h"

#include <algorithm>

namespace pws {

    // Ook nodig bij Arduino-toolchains die standaard C++11 gebruiken.
    constexpr int SnakeModel::MAX_COLUMNS;
    constexpr int SnakeModel::MAX_ROWS;

    void SnakeModel::reset(uint32_t random, int columns, int rows) {
        columns_ = std::max(4, std::min(columns, MAX_COLUMNS));
        rows_ = std::max(2, std::min(rows, MAX_ROWS));
        length_ = 3;
        turnCount_ = 0;
        direction_ = SnakeDirection::Right;
        status_ = SnakeStatus::Playing;
        for (int i = 0; i < length_; ++i) {
            body_[i] = {columns_ / 2 - i, rows_ / 2};
        }
        placeFood(random);
    }

    bool SnakeModel::occupies(SnakeCell cell) const {
        for (int i = 0; i < length_; ++i) {
            if (body_[i] == cell) return true;
        }
        return false;
    }

    void SnakeModel::queueDirection(SnakeDirection next) {
        if (status_ != SnakeStatus::Playing || turnCount_ == 2) return;
        const SnakeDirection previous = turnCount_ > 0 ? turns_[turnCount_ - 1] : direction_;
        const int difference = (static_cast<int>(next) - static_cast<int>(previous) + 4) % 4;
        if (difference == 0 || difference == 2) return;

        // Hoogstens twee bochten bewaren, maar slechts een bocht per bewegingsstap.
        turns_[turnCount_++] = next;
    }

    void SnakeModel::step(uint32_t random) {
        if (status_ != SnakeStatus::Playing) return;
        if (turnCount_ > 0) {
            direction_ = turns_[0];
            if (turnCount_ == 2) turns_[0] = turns_[1];
            --turnCount_;
        }

        SnakeCell next = body_[0];
        switch (direction_) {
            case SnakeDirection::Up: --next.y; break;
            case SnakeDirection::Right: ++next.x; break;
            case SnakeDirection::Down: ++next.y; break;
            case SnakeDirection::Left: --next.x; break;
        }

        if (next.x < 0 || next.y < 0 || next.x >= columns_ || next.y >= rows_) {
            status_ = SnakeStatus::Lost;
            return;
        }

        const bool grows = next == food_;
        // Zonder groei schuift de staart weg: die cel mag het nieuwe hoofd worden.
        const int collisionLength = grows ? length_ : length_ - 1;
        for (int i = 0; i < collisionLength; ++i) {
            if (next == body_[i]) {
                status_ = SnakeStatus::Lost;
                return;
            }
        }

        if (grows) ++length_;
        for (int i = length_ - 1; i > 0; --i) body_[i] = body_[i - 1];
        body_[0] = next;

        if (length_ == columns_ * rows_) {
            status_ = SnakeStatus::Won;
        } else if (grows) {
            placeFood(random);
        }
    }

    void SnakeModel::placeFood(uint32_t random) {
        const int freeCells = columns_ * rows_ - length_;
        int chosen = static_cast<int>(random % static_cast<uint32_t>(freeCells));

        // Selecteer een vrije cel; geen steeds trager wordende goklus bij een vol bord.
        for (int y = 0; y < rows_; ++y) {
            for (int x = 0; x < columns_; ++x) {
                const SnakeCell cell{x, y};
                if (!occupies(cell) && chosen-- == 0) {
                    food_ = cell;
                    return;
                }
            }
        }
    }

} // namespace pws
