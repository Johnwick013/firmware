# ESP32-S3-DevKitC-1U N16R8 + 4 inch ILI9488

This profile targets an Espressif ESP32-S3-DevKitC-1U with 16MB flash and 8MB OPI PSRAM,
wired to a 4 inch SPI TFT using an ILI9488 controller. Some listings typo this as
"ILI4688"; use the same profile if your display is actually ILI9488.

## TFT wiring

| TFT signal | ESP32-S3 GPIO |
| --- | --- |
| VCC | 3V3 |
| GND | GND |
| SCK / CLK | GPIO12 |
| MOSI / SDI | GPIO11 |
| MISO / SDO | GPIO13 |
| CS | GPIO10 |
| DC / RS | GPIO9 |
| RST | GPIO14 |
| BL / LED | GPIO21 |

## Optional XPT2046 touch wiring

Wire the touch controller to the same SPI bus as the TFT:

| Touch signal | ESP32-S3 GPIO |
| --- | --- |
| T_CLK | GPIO12 |
| T_DIN | GPIO11 |
| T_DO | GPIO13 |
| T_CS | GPIO8 |

The first boot with touch enabled opens TFT_eSPI calibration and saves the result to LittleFS.

## Notes

- Do not use GPIO26-GPIO37 on N16R8 modules; they can be reserved for flash/PSRAM.
- GPIO19/GPIO20 are the native USB D-/D+ pins, so this profile leaves them free.
- The SPI bus can be shared with a CC1101/NRF24 module by using separate chip-select pins.
