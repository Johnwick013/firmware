# Bruce Ultra Compact — provisional pin map

Synced with `boards/bruce-ultra/pins_arduino.h`.  
**Treat as draft until the schematic is frozen.** Do not flash `env:bruce-ultra` onto Reaper or other boards.

## Buses

| Bus | Pins | Used by |
|---|---|---|
| SPI0 | SCK 17, MOSI 18, MISO 8 | Display, SD, CC1101, nRF24, ST25R3916 |
| I2C | SDA 47, SCL 48 | AW9523, BQ25896, BQ27220, Grove |
| UART0 | TX 43, RX 44 | Console / BadUSB path |
| UART1 | TX 1, RX 2 | LF 125 kHz frontend |

## Chip selects / radio control

| Signal | GPIO | Notes |
|---|---|---|
| TFT_CS | 7 | Hold HIGH at boot |
| SDCARD_CS | 3 | Hold HIGH at boot |
| CC1101_SS | 9 | SubGHz |
| CC1101_GDO0 | 46 | |
| NRF24_SS | 13 | |
| NRF24_CE | 14 | |
| NFC_SS (ST25) | 11 | `SPI_SS_PIN` |
| LORA_CS (future) | 4 | Reserved / DNP |

## Display (ST7789 170×320)

| Signal | GPIO |
|---|---|
| BL | 6 |
| RST | 16 |
| DC | 15 |

## Buttons

| Button | GPIO | Active |
|---|---|---|
| SEL / OK | 0 | LOW |
| UP | 41 | LOW |
| DOWN | 40 | LOW |
| RIGHT | 38 | LOW |
| LEFT | 39 | LOW |
| ESC / power-hold | 21 | LONG → power off |

## RF path enables (AW9523)

| Expander pin | Function |
|---|---|
| 9 | CC1101 RX path enable |
| 10 | CC1101 TX path enable |
| 5 | Optional GPS power (DNP on v1) |
| 15 | Vibro |
| 0 | Logo / status LED |

## Antenna ports (mechanical, not GPIO)

| Port | Radio | Connector |
|---|---|---|
| ANT_WIFI | ESP32-S3-WROOM-1U | IPEX → RP-SMA |
| ANT_SUBGHZ | E07 CC1101 module | u.FL → RP-SMA |
| ANT_2G4 | E01 nRF24 module | u.FL → RP-SMA/MMCX |

Firmware should avoid concurrent high-duty Wi‑Fi TX and nRF24 TX.
