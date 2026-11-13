
#include "InputSystem.h"

namespace pws {

    static constexpr uint8_t knopPinnen[] = {
        PIN_KNOP_D_UP,
        PIN_KNOP_D_DOWN,
        PIN_KNOP_D_LEFT,
        PIN_KNOP_D_RIGHT,
        PIN_KNOP_A,
        PIN_KNOP_B,
        PIN_KNOP_X,
        PIN_KNOP_Y,
        PIN_KNOP_START
    };

    // Controleer of iedere knop een GPIO-pin heeft
    static_assert(
        sizeof(knopPinnen) / sizeof(knopPinnen[0])
            == AANTAL_KNOPPEN,
        "Aantal GPIO-pinnen komt niet overeen met aantal knoppen!"
    );

    void InputSystem::begin() {
        const uint32_t nu = millis();

        for (std::size_t i = 0; i < AANTAL_KNOPPEN; i++) {
            pinMode(knopPinnen[i], INPUT_PULLUP);

            knopStates[i] = digitalRead(knopPinnen[i]) == LOW;
            vorigeKnopStates[i] = knopStates[i];
            ruweKnopStates[i] = knopStates[i];
            laatsteVerandering[i] = nu;
        }
    }

    void InputSystem::updateKnopStates() {
        const uint32_t nu = millis();

        for (std::size_t i = 0; i < AANTAL_KNOPPEN; i++) {
            // Bewaar de stabiele toestand van het vorige gameframe.
            vorigeKnopStates[i] = knopStates[i];

            const bool ruweToestand =
                digitalRead(knopPinnen[i]) == LOW;

            // Elke gemeten overgang start de wachttijd voor deze knop opnieuw.
            if (ruweToestand != ruweKnopStates[i]) {
                ruweKnopStates[i] = ruweToestand;
                laatsteVerandering[i] = nu;
            }

            // Filter zowel indrukken als loslaten, zonder de gameloop te blokkeren.
            // Unsigned aftrekken blijft ook werken wanneer millis() rondloopt.
            if (static_cast<uint32_t>(nu - laatsteVerandering[i])
                >= ONTDENDER_TIJD_MS) {
                knopStates[i] = ruweKnopStates[i];
            }
        }
    }

    bool InputSystem::knopGedrukt(Knop knop) const {
        std::size_t index = static_cast<std::size_t>(knop);

        if (index >= AANTAL_KNOPPEN) {
            return false;
        }

        return knopStates[index];
    }

    bool InputSystem::knopZojuistGedrukt(Knop knop) const {
        std::size_t index = static_cast<std::size_t>(knop);

        if (index >= AANTAL_KNOPPEN) {
            return false;
        }

        return knopStates[index] && !vorigeKnopStates[index];
    }

}
