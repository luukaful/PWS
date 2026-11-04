#ifndef PWS_INPUTSYSTEM_H
#define PWS_INPUTSYSTEM_H

#include <Arduino.h>

// Knop constanten
#define KNOP_D_UP 0
#define KNOP_D_DOWN 1
#define KNOP_D_LEFT 2
#define KNOP_D_RIGHT 3
#define KNOP_A 4
#define KNOP_B 5
#define KNOP_X 6
#define KNOP_Y 7

// Knop pins (GPIO)
#define PIN_KNOP_D_UP 4
#define PIN_KNOP_D_DOWN 5
#define PIN_KNOP_D_LEFT 6
#define PIN_KNOP_D_RIGHT 7
#define PIN_KNOP_A 15
#define PIN_KNOP_B 16
#define PIN_KNOP_X 17
#define PIN_KNOP_Y 18

namespace pws {
    class InputSystem {
    private:
        static constexpr uint8_t AANTAL_KNOPPEN = 8;

        bool knopStates[AANTAL_KNOPPEN] = {};
        bool vorigeKnopStates[AANTAL_KNOPPEN] = {};

    public:
        void begin();
        void updateKnopStates();

        [[nodiscard]] bool knopGedrukt(int knop) const;
        [[nodiscard]] bool knopZojuistGedrukt(int knop) const;
    };
}

#endif // PWS_INPUTSYSTEM_H
