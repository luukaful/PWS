#include "InputSystem.h"

#include <Arduino.h>

namespace pws {

    // De volgorde moet overeenkomen met de waarden in enum class Knop.
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

    static_assert(
        sizeof(knopPinnen) / sizeof(knopPinnen[0]) == AANTAL_KNOPPEN,
        "Aantal GPIO-pinnen komt niet overeen met aantal knoppen!"
    );

    void InputSystem::begin() {
        const uint32_t nuMs = millis();

        for (std::size_t i = 0; i < AANTAL_KNOPPEN; ++i) {
            pinMode(knopPinnen[i], INPUT_PULLUP);

            KnopStatus& status = knopStatus[i];
            status.gedrukt = digitalRead(knopPinnen[i]) == LOW;

            // Een knop die bij het opstarten al ingedrukt is, telt niet als nieuwe druk.
            status.vorigFrameGedrukt = status.gedrukt;
            status.ruweToestand = status.gedrukt;
            status.laatsteVeranderingMs = nuMs;
        }
    }

    void InputSystem::updateKnopStates() {
        const uint32_t nuMs = millis();

        for (std::size_t i = 0; i < AANTAL_KNOPPEN; ++i) {
            KnopStatus& status = knopStatus[i];
            status.vorigFrameGedrukt = status.gedrukt;

            const bool ruweToestand = digitalRead(knopPinnen[i]) == LOW;

            // Elke gemeten overgang start de wachttijd voor deze knop opnieuw.
            if (ruweToestand != status.ruweToestand) {
                status.ruweToestand = ruweToestand;
                status.laatsteVeranderingMs = nuMs;
            }

            // Filter zowel indrukken als loslaten, zonder de gameloop te blokkeren.
            // Unsigned aftrekken blijft ook werken wanneer millis() rondloopt.
            const uint32_t stabieleTijdMs = nuMs - status.laatsteVeranderingMs;
            if (stabieleTijdMs >= ONTDENDER_TIJD_MS) {
                status.gedrukt = status.ruweToestand;
            }
        }
    }

    bool InputSystem::knopGedrukt(Knop knop) const {
        const std::size_t index = static_cast<std::size_t>(knop);

        if (index >= AANTAL_KNOPPEN) {
            return false;
        }

        return knopStatus[index].gedrukt;
    }

    bool InputSystem::knopZojuistGedrukt(Knop knop) const {
        const std::size_t index = static_cast<std::size_t>(knop);

        if (index >= AANTAL_KNOPPEN) {
            return false;
        }

        const KnopStatus& status = knopStatus[index];
        return status.gedrukt && !status.vorigFrameGedrukt;
    }

} // namespace pws
