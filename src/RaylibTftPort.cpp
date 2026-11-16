#include "RaylibTftPort.h"

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp_heap_caps.h>
#include <esp_log.h>

// Eerst buiten het extern "C"-blok in de port-header laden.
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <raylib_esp32_port.h>

#include <algorithm>
#include <cstring>

namespace {

    constexpr int RIJEN_PER_BLOK = 8;
    TFT_eSPI* tft = nullptr;
    uint16_t* pixelBlok = nullptr;
    uint16_t* laatstVerstuurd = nullptr;
    uint16_t breedte = 0;
    uint16_t hoogte = 0;
    bool referentieGeldig = false;
    pws::SchermTransferMeting meting;

} // namespace

namespace pws {

    bool verbindRaylibScherm(TFT_eSPI& scherm) {
        if (tft != nullptr || scherm.width() <= 0 || scherm.height() <= 0) {
            return false;
        }

        const size_t rijBytes = static_cast<size_t>(scherm.width()) * sizeof(uint16_t);
        auto* nieuwBlok = static_cast<uint16_t*>(
            heap_caps_malloc(rijBytes * RIJEN_PER_BLOK,
                             MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
        if (nieuwBlok == nullptr) {
            return false;
        }

        auto* nieuweReferentie = static_cast<uint16_t*>(
            heap_caps_malloc(rijBytes * scherm.height(),
                             MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
        if (nieuweReferentie == nullptr) {
            heap_caps_free(nieuwBlok);
            return false;
        }

        pixelBlok = nieuwBlok;
        laatstVerstuurd = nieuweReferentie;
        breedte = scherm.width();
        hoogte = scherm.height();
        referentieGeldig = false; // Het eerste frame gaat altijd volledig naar de TFT.
        meting = {};
        tft = &scherm;
        return true;
    }

    SchermTransferMeting laatsteSchermTransfer() {
        return meting;
    }

} // namespace pws

// Deze C-functies worden door rcore_esp32.c aangeroepen op de gameloop-task.
extern "C" {

    void raylib_port_get_dimensions(uint16_t* w, uint16_t* h) {
        if (w != nullptr) *w = breedte;
        if (h != nullptr) *h = hoogte;
    }

    esp_err_t raylib_port_wait_flush(void) {
        // Ondanks de API-naam flush_async is deze TFT-overdracht synchroon.
        return tft != nullptr ? ESP_OK : ESP_ERR_INVALID_STATE;
    }

    esp_err_t raylib_port_flush_async(const uint16_t* pixels, int w, int h) {
        meting = {};
        if (tft == nullptr || pixelBlok == nullptr || laatstVerstuurd == nullptr) {
            return ESP_ERR_INVALID_STATE;
        }
        if (pixels == nullptr || w != breedte || h != hoogte) {
            ESP_LOGE("PWS", "Raylib-framebuffer heeft onverwachte afmetingen");
            return ESP_ERR_INVALID_ARG;
        }

        const uint32_t vergelijkStart = micros();
        int x0 = 0, y0 = 0, x1 = w, y1 = h;

        if (referentieGeldig) {
            x0 = w;
            y0 = h;
            x1 = y1 = 0;

            for (int y = 0; y < h; ++y) {
                // Beide buffers hebben raylibs rijvolgorde: onderste rij eerst.
                const size_t offset = static_cast<size_t>(h - 1 - y) * w;
                const uint16_t* nieuw = pixels + offset;
                const uint16_t* oud = laatstVerstuurd + offset;

                if (std::memcmp(nieuw, oud, w * sizeof(uint16_t)) == 0) {
                    continue;
                }

                int links = 0;
                while (nieuw[links] == oud[links]) ++links;
                int rechts = w;
                while (nieuw[rechts - 1] == oud[rechts - 1]) --rechts;

                x0 = std::min(x0, links);
                x1 = std::max(x1, rechts);
                y0 = std::min(y0, y);
                y1 = y + 1;
            }
        }
        meting.vergelijkenUs = micros() - vergelijkStart;

        if (x0 >= x1 || y0 >= y1) {
            return ESP_OK; // Geen SPI-transactie voor een onveranderd beeld.
        }

        const uint32_t verstuurStart = micros();
        const int gebiedBreedte = x1 - x0;
        const bool vorigeSwap = tft->getSwapBytes();
        tft->setSwapBytes(true);
        tft->startWrite();

        for (int y = y0; y < y1; y += RIJEN_PER_BLOK) {
            const int rijen = std::min(RIJEN_PER_BLOK, y1 - y);
            for (int rij = 0; rij < rijen; ++rij) {
                const size_t offset = static_cast<size_t>(h - 1 - y - rij) * w + x0;
                std::memcpy(pixelBlok + rij * gebiedBreedte, pixels + offset,
                            gebiedBreedte * sizeof(uint16_t));
            }

            tft->pushImage(x0, y, gebiedBreedte, rijen, pixelBlok);

            // Bewaar eigen pixeldata: raylib wisselt tussen renderbuffers.
            // Een pointer naar de vorige renderbuffer is geen betrouwbare referentie.
            for (int rij = 0; rij < rijen; ++rij) {
                const size_t offset = static_cast<size_t>(h - 1 - y - rij) * w + x0;
                std::memcpy(laatstVerstuurd + offset, pixels + offset,
                            gebiedBreedte * sizeof(uint16_t));
            }
        }

        tft->endWrite();
        tft->setSwapBytes(vorigeSwap);
        referentieGeldig = true;
        meting.versturenUs = micros() - verstuurStart;
        meting.verstuurdePixels = static_cast<uint32_t>(gebiedBreedte) * (y1 - y0);
        return ESP_OK;
    }

} // extern "C"
