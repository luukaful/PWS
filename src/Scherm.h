#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <cstdint>
#include <TFT_eSPI.h>
#include <raylib.h>

namespace pws {

    class Scherm {
    public:
        bool begin();

        // Elk frame begint leeg; het menu/spel tekent het volledige beeld.
        void beginFrame(Color achtergrond = BLACK);
        void endFrame();

        [[nodiscard]] int16_t getBreedte() const;
        [[nodiscard]] int16_t getHoogte() const;

    private:
        TFT_eSPI tft;
        int16_t breedte = 0;
        int16_t hoogte = 0;
    };

} // namespace pws

#endif
