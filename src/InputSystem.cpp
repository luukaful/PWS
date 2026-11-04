#include "InputSystem.h"

namespace pws {

    // GPIO-pin voor iedere knop
    static constexpr uint8_t knopPinnen[8] = {
        PIN_KNOP_D_UP,      // 0
        PIN_KNOP_D_DOWN,    // 1
        PIN_KNOP_D_LEFT,    // 2
        PIN_KNOP_D_RIGHT,   // 3
        PIN_KNOP_A,         // 4
        PIN_KNOP_B,         // 5
        PIN_KNOP_X,         // 6
        PIN_KNOP_Y          // 7
    };

    void InputSystem::begin() {
        for (int i = 0; i < AANTAL_KNOPPEN; i++) {
            pinMode(knopPinnen[i], INPUT_PULLUP);

            // Lees direct de huidige toestand
            knopStates[i] = digitalRead(knopPinnen[i]) == LOW;
            vorigeKnopStates[i] = knopStates[i];
        }
    }

    void InputSystem::updateKnopStates() {
        for (int i = 0; i < AANTAL_KNOPPEN; i++) {
            vorigeKnopStates[i] = knopStates[i];

            knopStates[i] = digitalRead(knopPinnen[i]) == LOW;
        }
    }

    bool InputSystem::knopGedrukt(int knop) const {
        if (knop < 0 || knop >= AANTAL_KNOPPEN) {
            return false;
        }

        return knopStates[knop];
    }

    bool InputSystem::knopZojuistGedrukt(int knop) const {
        if (knop < 0 || knop >= AANTAL_KNOPPEN) {
            return false;
        }

        return knopStates[knop] && !vorigeKnopStates[knop];
    }

}
