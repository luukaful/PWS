#ifndef PWS_RAYLIB_TFT_PORT_H
#define PWS_RAYLIB_TFT_PORT_H

#include <cstdint>

class TFT_eSPI;

namespace pws {

    struct SchermTransferMeting {
        uint32_t vergelijkenUs = 0;
        uint32_t versturenUs = 0; // Inclusief kopieren en bijwerken van de referentie.
        uint32_t verstuurdePixels = 0;
    };

    // Het scherm moet al geinitialiseerd en gedraaid zijn en blijven bestaan.
    // Aanroepen voordat raylib InitWindow() uitvoert.
    bool verbindRaylibScherm(TFT_eSPI& scherm);

    // Meting van de laatste flush; ook een onveranderd beeld heeft een meting.
    SchermTransferMeting laatsteSchermTransfer();

} // namespace pws

#endif
