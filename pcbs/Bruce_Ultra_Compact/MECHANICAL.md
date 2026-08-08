# Mechanical / RF keepouts

## Target shell
- Outer envelope: 95 × 42 × 16 mm
- Wall: 1.2 mm PETG / ABS
- Display window: 1.47" centered upper half
- 5-way nest below display; ESC top-left

## Antenna keepouts
- No large ground pour within 5 mm of SubGHz SMA feed
- Wi‑Fi and nRF SMA bulkheads on **opposite** short edges (≥ 70 mm separation on this chassis)
- NFC coil: keep LiPo and USB-C shield outside the inner 60% of the loop
- Metal belt clips / screws only on mid-frame rails, not behind coil

## Pocket vs ultra antennas
- Pocket: 20–40 mm helical/stub on all three ports
- Ultra: 433 MHz ~17 cm whip on SubGHz; 2.4 GHz 10–12 cm on Wi‑Fi; leave nRF stub or match

## Thermal
- E01 27 dBm PA needs copper pour + case vent near module
- Duty-cycle limit high-power TX in firmware/UI warnings
