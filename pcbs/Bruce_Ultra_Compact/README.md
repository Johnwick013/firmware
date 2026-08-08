# Bruce Ultra Compact

Hardware target: a pocket-sized Bruce device that matches Flipper Zero coverage, then pushes RF further with proper antennas and higher-power frontends — without becoming a brick.

**Status:** design / BOM / firmware board scaffold (`boards/bruce-ultra`). PCB layout not yet released.

**Intended use:** authorized security research, red-team labs, and learning. Transmit only where you are legally allowed to.

---

## Goal vs Flipper Zero / Bruce Reaper

| Capability | Flipper Zero | Bruce RF Reaper | Ultra Compact (this) |
|---|---|---|---|
| Form factor | ~100×40×25 mm | Devkit, larger | Target ≤ 95×42×16 mm |
| SubGHz | CC1101 ~10 dBm, built-in antenna | E07-433M20S 20 dBm | 20 dBm + RP-SMA / MMCX + optional whip |
| Wi‑Fi sniff | No native Wi‑Fi toolset | ESP32-S3 PCB / module antenna | ESP32-S3-WROOM-**1U** IPEX → RP-SMA |
| 2.4 GHz RF | nRF24 via GPIO/addons | E01-2G4M27SX 27 dBm | Same class PA + dedicated SMA |
| HF NFC | ST25R3916-class | ST25R3916 | ST25R3916 + larger perimeter coil |
| LF 125 kHz | Native | External / limited | On-board UART LF reader/writer |
| Emulate | LF + HF | HF (ST25) path | HF native + LF via dedicated frontend |

Antenna quality beats “more chips”. A 20 dBm CC1101 into a PCB trace often loses to a 10 dBm radio into a real whip. This design prioritizes **connectors + isolation** over stuffing every peripheral.

---

## Architecture (one composition)

```
                 ┌──────────────────────────────────────┐
   WiFi SMA ───► │ ESP32-S3-WROOM-1U (N16R8)  IPEX     │
                 │  WiFi 2.4 / BLE / USB-HID / UI       │
                 └───────────────┬──────────────────────┘
                                 │ SPI + I2C + UART
          ┌──────────────────────┼──────────────────────┐
          ▼                      ▼                      ▼
   ┌─────────────┐       ┌─────────────┐        ┌──────────────┐
   │ E07-xxxM20S │       │ E01-2G4M27  │        │ ST25R3916    │
   │ CC1101 20dBm│       │ nRF24 27dBm │        │ HF NFC R/W/E │
   │ → SubGHz SMA│       │ → 2.4 SMA   │        │ perimeter coil│
   └─────────────┘       └─────────────┘        └──────────────┘
          ▲
          │ optional dual-band: 433 module OR 868/915 module
          │ (same footprint, BOM option)

   UART LF 125 kHz frontend ── read / write / emulate (T55xx class)
   IR TX/RX · microSD · BQ25896 + fuel gauge · 800–1000 mAh
   1.47" ST7789 172×320 · 5-way + ESC
```

### Why three separate RF antennas

1. **Wi‑Fi SMA** — sniffing / AP / wardrive range is antenna-limited on every Stick/Cardputer.
2. **SubGHz SMA** — 433/868/915 needs electrical length; short helical for pocket, whip for “ultra”.
3. **2.4 GHz nRF SMA** — keep nRF24 PA away from the ESP antenna so Wi‑Fi and mousejack paths do not detune each other.

Do **not** share one 2.4 GHz antenna between ESP32 and nRF24 without an RF switch and firmware mutex.

---

## Compact mechanical plan

Target enclosure (3D-print first):

- Outer: **95 × 42 × 16 mm** (Flipper-class pocket size, slightly thinner)
- Front: display + 5-way nest + ESC
- Top edge: USB-C
- Left short edge: SubGHz RP-SMA (bulkhead)
- Right short edge: Wi‑Fi RP-SMA
- Bottom edge: 2.4 GHz RP-SMA (or MMCX pigtail to keep thinner)
- Back: NFC coil (keep metal/battery away from coil center)
- Battery: 3.7 V 800–1000 mAh flat LiPo under PCB or beside coil

**Pocket mode:** short helical / stub antennas.  
**Ultra mode:** screw on 433 MHz 1/4-wave whip + 2.4 GHz dipole. Same PCB, no redesign.

---

## Radio choices (the “more extreme” part)

### SubGHz
- Default module: **E07-433M20S** (CC1101, +20 dBm) — already Bruce-supported.
- EU868 / US915 BOM variant: **E07-900M20S** same footprint.
- FE path: module u.FL → short coax → board RP-SMA (not a long PCB microstrip).
- Optional later: discrete PA/LNA frontend (e.g. CC1190-class) — only if legal TX limits and heat allow; v1 stays module-based.

### Wi‑Fi
- MCU module **must** be **WROOM-1U** (IPEX), not PCB-antenna WROOM-1.
- External 2.4 GHz antenna: 3–5 dBi stub for compact, higher-gain omni when stationary.
- For 5 GHz Wi‑Fi: track Bruce 2.0 **ESP32-C5** boards separately; do not force C5 into v1 until RF + UI stack is stable.

### RFID / NFC
- **HF:** ST25R3916 (same class as Reaper / T-LoRa Pager) — stronger field than PN532 for read/emulate distance.
- Coil: larger rectangular loop under back shell (trade thickness for range).
- **LF 125 kHz:** compact UART reader/writer (PN532Killer-compatible or dedicated EM4095-class frontend) so Flipper-style LF copy/emulate is on-board, not a Grove dongle.

### IR / storage / power
- IR TX + RX on front bezel
- microSD on SPI bus (shared with RF CS discipline — see pin map)
- BQ25896 charger + BQ27220 gauge, ~1000 mAh design capacity
- AW9523 IO expander for vibro / RF path enables / status LEDs (same pattern as Reaper)

---

## Prototype path (build something this week)

Before spinning a custom PCB:

1. Flash Bruce on **RF Reaper** or **T-Embed CC1101** (closest feature parity).
2. Replace stock antennas with **u.FL→SMA pigtails** + known-good whips — measure RSSI yourself.
3. Add **PN532Killer** or LF UART module for 125 kHz.
4. Freeze what actually wins range/size, then lay out Ultra Compact v1.

Firmware board env for the custom PCB: `bruce-ultra` (see `boards/bruce-ultra/`). Pins are provisional until the schematic is locked — do not flash that env onto Reaper hardware.

---

## Files

| File | Purpose |
|---|---|
| `README.md` | This architecture |
| `BOM.csv` | v1 parts list |
| `PINMAP.md` | Provisional GPIO map (matches `boards/bruce-ultra`) |
| `MECHANICAL.md` | Enclosure / antenna keepouts |

Upstream references: [Bruce boards](https://bruce.computer/boards), existing `pcbs/Bruce_PCB_smoochie`, `boards/reaper`.
