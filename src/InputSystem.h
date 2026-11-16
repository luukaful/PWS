#ifndef PWS_INPUTSYSTEM_H
#define PWS_INPUTSYSTEM_H

#include <cstddef>
#include <cstdint>

// De knoppen verbinden hun GPIO-pin met GND; INPUT_PULLUP houdt ze anders hoog.
constexpr uint8_t PIN_KNOP_D_UP = 4;
constexpr uint8_t PIN_KNOP_D_DOWN = 5;
constexpr uint8_t PIN_KNOP_D_LEFT = 6;
constexpr uint8_t PIN_KNOP_D_RIGHT = 7;
constexpr uint8_t PIN_KNOP_A = 15;
constexpr uint8_t PIN_KNOP_B = 16;
constexpr uint8_t PIN_KNOP_X = 17;
constexpr uint8_t PIN_KNOP_Y = 18;
constexpr uint8_t PIN_KNOP_START = 8;

namespace pws {

    enum class Knop : uint8_t {
        D_UP,
        D_DOWN,
        D_LEFT,
        D_RIGHT,
        A,
        B,
        X,
        Y,
        START,

        AANTAL_KNOPPEN
    };

    constexpr std::size_t AANTAL_KNOPPEN =
        static_cast<std::size_t>(Knop::AANTAL_KNOPPEN);

    class InputSystem {
    public:
        void begin();

        // Eenmaal per gameframe aanroepen, voordat de game de knoppen uitleest.
        void updateKnopStates();

        [[nodiscard]] bool knopGedrukt(Knop knop) const;

        // Blijft het hele frame waar; uitlezen verbruikt de drukgebeurtenis niet.
        [[nodiscard]] bool knopZojuistGedrukt(Knop knop) const;

    private:
        static constexpr uint32_t ONTDENDER_TIJD_MS = 20;

        struct KnopStatus {
            bool gedrukt = false;
            bool vorigFrameGedrukt = false;
            bool ruweToestand = false;
            uint32_t laatsteVeranderingMs = 0;
        };

        KnopStatus knopStatus[AANTAL_KNOPPEN] = {};
    };

} // namespace pws

#endif // PWS_INPUTSYSTEM_H
