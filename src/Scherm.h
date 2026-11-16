#ifndef PWS_SCHERM_H
#define PWS_SCHERM_H

#include <cstdint>
#include <TFT_eSPI.h>

namespace pws {

    class Scherm {
    public:
        bool begin();

        // Start een nieuwe verzameling wijzigingen; bestaande pixels blijven bewaard.
        void beginFrame();

        // Verstuurt het gewijzigde gebied en geeft aan of dat gelukt is.
        // Zonder wijzigingen wordt niets verstuurd en is het resultaat false.
        bool endFrame();

        // Wis de framebuffer en markeer het hele scherm voor verzending.
        void clear(uint16_t kleur = TFT_BLACK);
        void markDirty(int16_t x, int16_t y, int16_t gebiedBreedte, int16_t gebiedHoogte);

        [[nodiscard]] TFT_eSPI& getTFT();

        // Tekenwerk in de sprite moet ook met markDirty() worden aangemeld.
        [[nodiscard]] TFT_eSprite& getSprite();

        [[nodiscard]] int16_t getBreedte() const;
        [[nodiscard]] int16_t getHoogte() const;

    private:
        TFT_eSPI tft;
        TFT_eSprite sprite{&tft};
        int16_t breedte = 0;
        int16_t hoogte = 0;

        bool dirty = false;
        int16_t dirtyX0 = 0;
        int16_t dirtyY0 = 0;

        // De rechter- en ondergrens zijn exclusief: breedte = X1 - X0.
        int16_t dirtyX1 = 0;
        int16_t dirtyY1 = 0;
    };

} // namespace pws

#endif // PWS_SCHERM_H
