//
// Created by luukd on 04/11/2026.
//

#include "InputSystem.h"

namespace pws {
    InputSystem::InputSystem() {
        pinMode(PIN_KNOP_D_UP, INPUT_PULLUP);
        pinMode(PIN_KNOP_D_DOWN, INPUT_PULLUP);
        pinMode(PIN_KNOP_D_LEFT, INPUT_PULLUP);
        pinMode(PIN_KNOP_D_RIGHT, INPUT_PULLUP);
        pinMode(PIN_KNOP_A, INPUT_PULLUP);
        pinMode(PIN_KNOP_B, INPUT_PULLUP);
        pinMode(PIN_KNOP_X, INPUT_PULLUP);
        pinMode(PIN_KNOP_Y, INPUT_PULLUP);

        knopStates[KNOP_D_UP] = false;
        knopStates[KNOP_D_DOWN] = false;
        knopStates[KNOP_D_LEFT] = false;
        knopStates[KNOP_D_RIGHT] = false;
        knopStates[KNOP_A] = false;
        knopStates[KNOP_B] = false;
        knopStates[KNOP_X] = false;
        knopStates[KNOP_Y] = false;
    }

    void InputSystem::updateKnopStates() {
        knopStates[KNOP_D_UP] = !digitalRead(PIN_KNOP_D_UP);
        knopStates[KNOP_D_DOWN] = !digitalRead(PIN_KNOP_D_DOWN);
        knopStates[KNOP_D_LEFT] = !digitalRead(PIN_KNOP_D_LEFT);
        knopStates[KNOP_D_RIGHT] = !digitalRead(PIN_KNOP_D_RIGHT);
        knopStates[KNOP_A] = !digitalRead(PIN_KNOP_A);
        knopStates[KNOP_B] = !digitalRead(PIN_KNOP_B);
        knopStates[KNOP_X] = !digitalRead(PIN_KNOP_X);
        knopStates[KNOP_Y] = !digitalRead(PIN_KNOP_Y);
    }

    bool InputSystem::knopGedrukt(int knop) const {
        auto it = knopStates.find(knop);
        if (it != knopStates.end()) {
            return it->second;
        }
        return false; // Als de knop niet bestaat, retourneer false
    }
}// pws