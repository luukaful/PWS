#ifndef PWS_TFT_SETUP_H
#define PWS_TFT_SETUP_H

// Overgenomen uit User_Setup_COPY.h. PlatformIO laadt deze instellingen
// ook tijdens het compileren van TFT_eSPI zelf.
#define USER_SETUP_LOADED
#define USER_SETUP_INFO "PWS raylib + TFT_eSPI"
#define ILI9341_DRIVER
#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST (-1)
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000

// Tekst wordt door raylib getekend; TFT_eSPI verstuurt alleen pixels.

#endif
