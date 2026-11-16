#ifndef PWS_SNAKE_MODEL_H
#define PWS_SNAKE_MODEL_H

#include <array>
#include <cstdint>

namespace pws {

    struct SnakeCell {
        int x;
        int y;
        constexpr SnakeCell(int column = 0, int row = 0) : x(column), y(row) {}
        bool operator==(const SnakeCell& other) const { return x == other.x && y == other.y; }
    };

    enum class SnakeDirection { Up, Right, Down, Left };
    enum class SnakeStatus { Playing, Lost, Won };

    // Spelregels zonder afhankelijkheid van raylib, Arduino of GPIO.
    class SnakeModel {
    public:
        static constexpr int MAX_COLUMNS = 20;
        static constexpr int MAX_ROWS = 14;

        void reset(uint32_t random, int columns = MAX_COLUMNS, int rows = MAX_ROWS);
        void queueDirection(SnakeDirection next);
        void step(uint32_t random);

        int columns() const { return columns_; }
        int rows() const { return rows_; }
        int length() const { return length_; }
        int score() const { return length_ - 3; }
        SnakeCell segment(int index) const { return body_[index]; }
        SnakeCell food() const { return food_; }
        SnakeDirection direction() const { return direction_; }
        SnakeStatus status() const { return status_; }
        bool occupies(SnakeCell cell) const;

    private:
        std::array<SnakeCell, MAX_COLUMNS * MAX_ROWS> body_{};
        std::array<SnakeDirection, 2> turns_{};
        int columns_ = MAX_COLUMNS;
        int rows_ = MAX_ROWS;
        int length_ = 3;
        int turnCount_ = 0;
        SnakeCell food_{};
        SnakeDirection direction_ = SnakeDirection::Right;
        SnakeStatus status_ = SnakeStatus::Playing;

        void placeFood(uint32_t random);
    };

} // namespace pws

#endif
