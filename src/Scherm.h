#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <cstdint>
#include <TFT_eSPI.h>

namespace pws {
class Scherm {
    TFT_eSPI tft = TFT_eSPI();
    int16_t hoogte = 0;
    int16_t breedte = 0;

    public:
    void begin();

    [[nodiscard]] TFT_eSPI& getTFT();
    [[nodiscard]] int16_t getHoogte() const;
    [[nodiscard]] int16_t getBreedte() const;
};
}

#endif // PWS_SCHERM_H
