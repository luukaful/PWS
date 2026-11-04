
#ifndef PWS_INPUTSYSTEM_H
#define PWS_INPUTSYSTEM_H

#include <Arduino.h>
#include <cstddef>
#include <cstdint>

// Knop pins (GPIO)
#define PIN_KNOP_D_UP 4
#define PIN_KNOP_D_DOWN 5
#define PIN_KNOP_D_LEFT 6
#define PIN_KNOP_D_RIGHT 7
#define PIN_KNOP_A 15
#define PIN_KNOP_B 16
#define PIN_KNOP_X 17
#define PIN_KNOP_Y 18
#define PIN_KNOP_START 8

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
    private:
        bool knopStates[AANTAL_KNOPPEN] = {};
        bool vorigeKnopStates[AANTAL_KNOPPEN] = {};

    public:
        void begin();
        void updateKnopStates();

        [[nodiscard]] bool knopGedrukt(Knop knop) const;
        [[nodiscard]] bool knopZojuistGedrukt(Knop knop) const;
    };

}

#endif // PWS_INPUTSYSTEM_H
