#ifndef PWS_RAYLIB_TFT_PORT_H
#define PWS_RAYLIB_TFT_PORT_H

class TFT_eSPI;

namespace pws {

    // Het scherm moet al geinitialiseerd en gedraaid zijn en blijven bestaan.
    // Aanroepen voordat raylib InitWindow() uitvoert.
    bool verbindRaylibScherm(TFT_eSPI& scherm);

} // namespace pws

#endif
