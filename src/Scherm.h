#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <cstdint>
#include <TFT_eSPI.h>

namespace pws {
    class Scherm {
        TFT_eSPI tft = TFT_eSPI();
        TFT_eSprite sprite = TFT_eSprite(&tft);
        int16_t hoogte = 0;
        int16_t breedte = 0;

        bool dirty = false;
        int16_t dirtyX0 = 0;
        int16_t dirtyY0 = 0;
        int16_t dirtyX1 = 0;
        int16_t dirtyY1 = 0;

    public:
        bool begin();

        void beginFrame();
        bool endFrame();

        // Wis de framebuffer en markeer het hele scherm voor verzending.
        void clear(uint16_t kleur = TFT_BLACK);

        [[nodiscard]] TFT_eSPI& getTFT();
        [[nodiscard]] TFT_eSprite& getSprite();

        [[nodiscard]] int16_t getHoogte() const;
        [[nodiscard]] int16_t getBreedte() const;

        void markDirty(int16_t x, int16_t y, int16_t w, int16_t h);
    };
}

#endif // PWS_SCHERM_H

