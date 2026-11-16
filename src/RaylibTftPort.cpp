#include "RaylibTftPort.h"

#include <TFT_eSPI.h>
#include <esp_heap_caps.h>
#include <esp_log.h>

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>

#include <raylib_esp32_port.h>

#include <algorithm>
#include <cstring>

namespace {

    constexpr int RIJEN_PER_BLOK = 8;
    TFT_eSPI* tft = nullptr;
    uint16_t* pixelBlok = nullptr;
    uint16_t breedte = 0;
    uint16_t hoogte = 0;

} // namespace

namespace pws {

    bool verbindRaylibScherm(TFT_eSPI& scherm) {
        if (tft != nullptr || scherm.width() <= 0 || scherm.height() <= 0) {
            return false;
        }

        const size_t bytes = static_cast<size_t>(scherm.width())
            * RIJEN_PER_BLOK * sizeof(uint16_t);
        pixelBlok = static_cast<uint16_t*>(
            heap_caps_malloc(bytes, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)
        );
        if (pixelBlok == nullptr) {
            return false;
        }

        tft = &scherm;
        breedte = scherm.width();
        hoogte = scherm.height();
        return true;
    }

} // namespace pws

// Deze drie C-functies vormen de aansluiting op rcore_esp32.c.
extern "C" {

    void raylib_port_get_dimensions(uint16_t* w, uint16_t* h) {
        if (w != nullptr) *w = breedte;
        if (h != nullptr) *h = hoogte;
    }

    esp_err_t raylib_port_wait_flush(void) {
        // pushImage() werkt hier synchroon; er staat geen DMA-taak open.
        return tft != nullptr ? ESP_OK : ESP_ERR_INVALID_STATE;
    }

    esp_err_t raylib_port_flush_async(const uint16_t* pixels, int w, int h) {
        if (tft == nullptr || pixelBlok == nullptr) {
            return ESP_ERR_INVALID_STATE;
        }
        if (pixels == nullptr || w != breedte || h != hoogte) {
            ESP_LOGE("PWS", "Raylib-framebuffer heeft onverwachte afmetingen");
            return ESP_ERR_INVALID_ARG;
        }

        const bool vorigeSwap = tft->getSwapBytes();
        tft->setSwapBytes(true);
        tft->startWrite();

        for (int y = 0; y < h; y += RIJEN_PER_BLOK) {
            const int rijen = std::min(RIJEN_PER_BLOK, h - y);
            for (int rij = 0; rij < rijen; ++rij) {
                // rlsw bewaart de onderste rij eerst; TFT_eSPI verwacht de bovenste.
                const uint16_t* bron = pixels + (h - 1 - y - rij) * w;
                std::memcpy(pixelBlok + rij * w, bron, w * sizeof(uint16_t));
            }
            tft->pushImage(0, y, w, rijen, pixelBlok);
        }

        tft->endWrite();
        tft->setSwapBytes(vorigeSwap);
        return ESP_OK;
    }

} // extern "C"
