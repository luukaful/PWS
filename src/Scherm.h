#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <stdint.h>
#include <TFT_eSPI.h>

namespace pws {
class Scherm {
    TFT_eSPI tft = TFT_eSPI();
    int16_t hoogte = 0;
    int16_t breedte = 0;

    public:
    Scherm();

    [[nodiscard]] TFT_eSPI& getTFT();
    [[nodiscard]] int16_t getHoogte() const;
    [[nodiscard]] int16_t getBreedte() const;
};
} // pws

#endif //PWS_SCHERM_H
