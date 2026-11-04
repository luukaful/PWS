#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <stdint.h>
#include <TFT_eSPI.h>

namespace pws {
class Scherm {
    TFT_eSPI tft = TFT_eSPI();
    int hoogte = 0;
    int breedte = 0;

    public:
    Scherm();

    [[nodiscard]] int getHoogte() const;
    [[nodiscard]] int getBreedte() const;
};
} // pws

#endif //PWS_SCHERM_H
