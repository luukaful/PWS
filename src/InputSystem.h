//
// Created by luukd on 04/11/2026.
//

#ifndef PWS_INPUTSYSTEM_H
#define PWS_INPUTSYSTEM_H
#include <unordered_map>
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
#define PIN_KNOP_D_UP 2
#define PIN_KNOP_D_DOWN 3
#define PIN_KNOP_D_LEFT 4
#define PIN_KNOP_D_RIGHT 5
#define PIN_KNOP_A 6
#define PIN_KNOP_B 7
#define PIN_KNOP_X 8
#define PIN_KNOP_Y 9

namespace pws {
    class InputSystem {
        std::unordered_map<int, bool> knopStates;

    public:
        InputSystem();
        void updateKnopStates();
        bool knopGedrukt(int knop) const;
    }; // pws
}

#endif //PWS_INPUTSYSTEM_H
