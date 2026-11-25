# TeensyRPN42 – BOM & order list

Generated 2026-11-25 from the schematic (`kicad-cli sch export bom`), checked against the Part-DB stock at parts.erck.be and the Reichelt shop. Prices: Reichelt, €/pc incl. VAT, on that date. Interactive placement: [TeensyRPN42_ibom.html](TeensyRPN42_ibom.html).

Legend: **Stock** = from your Part-DB (name, qty in stock, location) · **Reichelt** = order code × qty to order · **elsewhere** = not available at Reichelt.

## Summary

- 43 BOM lines, 168 placed parts.
- **From stock only:** 15 lines (all resistors, 100 nF, 1 nF, 18 pF, MCU, OLED, Choc switches).
- **Reichelt:** 22 positions, ≈ 8.64 € (without shipping).
- **Elsewhere:** Y2 (32.768 kHz 3215 CL 6–7 pF; Reichelt alternatives: `32,768 CC7V-9` with 12 pF caps, or the 85SMX on Y4), SK6805 2020 LEDs (already ordered).

## Reichelt order list

Copy into the Reichelt "Schnellbestellung" (code + qty).

| Reichelt code | Qty | €/pc | € | For |
|---|--:|--:|--:|---|
| `2N7002` | 1 | 0.06 | 0.06 | Q2 |
| `74AHC T1G125GW` | 1 | 0.20 | 0.20 | U3 |
| `BLM18AG601SN1D` | 1 | 0.08 | 0.08 | L1 |
| `BLM18KG601SN1D` | 2 | 0.10 | 0.20 | L2, L3 |
| `CL10C6R8CB8NNNC` | 2 | 0.03 | 0.06 | C32, C33 |
| `DIP DTSM-61K` | 2 | 0.30 | 0.60 | SW1, SW2 |
| `GRM31CC81E226K` | 5 | 0.23 | 1.15 | C4, C5, C37, C38, C53 |
| `IC14M8` | 1 | 1.10 | 1.10 | Y3 |
| `IRLML 6401` | 1 | 0.72 | 0.72 | Q1 |
| `JST EH2P ST90` | 2 | 0.10 | 0.20 | J5, J9 |
| `JUMPER 2,54 SW` | 1 | 0.02 | 0.02 | J10 |
| `KEM X5R0603 2,2U` | 2 | 0.04 | 0.08 | 2 of C27/C28/C30/C31/C34/C35 (4 from stock) |
| `L-242418FPS 6,3µ` | 1 | 0.42 | 0.42 | L4 |
| `MCP17273302ESN` | 1 | 1.15 | 1.15 | U4 |
| `MIC USB BBU` | 1 | 0.71 | 0.71 | J7 |
| `MPE 006-1-004` | 2 | 0.10 | 0.20 | J2, J8 |
| `SK 34A SMD` | 1 | 0.17 | 0.17 | D42 |
| `SL 1X36G 2,54` | 1 | 0.30 | 0.30 | J1, J4, J10 (break off) |
| `SMAJ 5.0A LITT` | 1 | 0.11 | 0.11 | D41 |
| `SMD1206P075TF16` | 1 | 0.21 | 0.21 | F1 |
| `TS1935BCX5` | 1 | 0.76 | 0.76 | U5 |
| `USBLC6-2SC6` | 1 | 0.14 | 0.14 | U6 |
| | | | **8.64** | |

Optional extras worth adding: a few spare 0603 parts of the cheap lines (2.2 µF, 6.8 pF), JST EH cable side for the TP4056 module wiring: `JST EH2P BU` ×2 + `JST EH CKS` ×4.

## Reichelt codes for the stock parts

For re-ordering or a second board. Qty = per board.

| Reichelt code | Qty | €/pc | For | Note |
|---|--:|--:|---|---|
| `CL10C180JB8NNNC` | 2 | 0.03 | C1, C2 |  |
| `X7R-G0603 100N` | 35 | 0.03 | C3, C6, C8–C11, C13–C25, C29, C40–C52, C55, C56 | 16 V X7R |
| `KEM C0G0603 1,0N` | 1 | 0.02 | C26 |  |
| `KEM X5R0603 2,2U` | 6 | 0.04 | C27, C28, C30, C31, C34, C35 |  |
| `SMD-0603 10K` | 4 | 0.02 | R1, R5, R23, R24 |  |
| `SMD-0603 470` | 1 | 0.02 | R2 |  |
| `SMD-0603 2,2K` | 2 | 0.02 | R3, R4 |  |
| `SMD-0603 68K` | 1 | 0.02 | R13 | 66.5 k not at Reichelt; 68 k with R14 22 k → 5.07 V |
| `SMD-0603 22K` | 1 | 0.02 | R14 |  |
| `SMD-0603 100K` | 3 | 0.02 | R15, R20, R21 |  |
| `SMD-0603 220K` | 2 | 0.02 | R16, R17 | 200 k not at Reichelt; any equal pair works (V_bat/2) |
| `SMD-0603 150K` | 1 | 0.02 | R22 |  |
| `STM32F405RGT6` | 1 | 6.95 | U2 |  |

Not at Reichelt: OLED module, Choc switches, SRU1048-100Y (L5), TP4056 module.

## Full BOM

| Qty | Refs | Part | Footprint | Stock (Part-DB) | Reichelt | Notes |
|--:|---|---|---|---|---|---|
| 2 | C1, C2 | 18 pF C0G 0603 | C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | GRM1885C1H180JA01J (20, Box SMD Reste) | stock · re-order: `CL10C180JB8NNNC` | was 20 pF; fine for the 8 MHz crystal |
| 35 | C3, C6, C8–C11, C13–C25, C29, C40–C52, C55, C56 | 100 nF 0603 | C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | CC0603KRX7R9BB104 (X7R) (27, Box SMD Reste)<br>C0603C104Z3VAC (Y5V) (128, Box SMD Reste) | stock · re-order: `X7R-G0603 100N` | X7R at the MCU/OLED, Y5V for the LED caps C14–C25, C40–C52 |
| 5 | C4, C5, C37, C38, C53 | 22 µF X5R/X6S 1206 | C_1206_3216Metric_Pad1.33x1.80mm_HandSolder | – | `GRM31CC81E226K` ×5 | stock CC1206MKY5V6BB226 is Y5V (≈5 µF left at 5 V), not used |
| 1 | C26 | 1 nF C0G 0603 | C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | GRM1885C1H102FA01J (7, Box SMD Reste) | stock · re-order: `KEM C0G0603 1,0N` | U4 power-good delay |
| 6 | C27, C28, C30, C31, C34, C35 | 2.2 µF X5R 0603 | C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | GRM188R61E225KA12D (4, Box SMD Reste) | `KEM X5R0603 2,2U` ×2 | 4 from stock + 2 to order |
| 2 | C32, C33 | 6.8 pF C0G 0603 | C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | – | `CL10C6R8CB8NNNC` ×2 | for Y2 (CL 6 pF); see Y2 note |
| 38 | D1–D38 | SK6805 2020 (2.0 × 2.0 mm, KAIHCHIP) | LED_SK6805_PLCC4_2.4x2.7mm_P1.3mm | – | – | ordered separately; fits the 2427 footprint (same pin order, terminals on the pads) |
| 1 | D41 | SMAJ5.0A TVS, SMA | D_SMA | – | `SMAJ 5.0A LITT` ×1 |  |
| 1 | D42 | Schottky 40 V 3 A, SMA | D_SMA | – | `SK 34A SMD` ×1 | SS34 equivalent |
| 1 | F1 | PTC 0.75 A hold, 1206 | Fuse_1206_3216Metric | – | `SMD1206P075TF16` ×1 |  |
| 1 | J1 | Pin header 1×6 (SWD) | PinHeader_1x06_P2.54mm_Vertical | – | `SL 1X36G 2,54` ×1 | one 36-pin strip covers J1, J4, J10 |
| 2 | J2, J8 | Socket 1×4, 2.54 mm | PinSocket_1x04_P2.54mm_Vertical | – | `MPE 006-1-004` ×2 |  |
| 1 | J3 | OLED 2.8" 256×64 SSD1322 | OLED-2.8inch16pin | OLED 2.8" 256x64 SSD1322 weiss (1, Ikea Schubbox) | – | set the module to 4-wire SPI (R19/R21 fitted, R18/R20 open) |
| 1 | J4 | Pin header 1×3 (serial) | PinHeader_1x03_P2.54mm_Vertical | – | – | from the J1 strip |
| 1 | J5 | JST EH 2-pin, 90° | JST_EH_S2B-EH_1x02_P2.50mm_Horizontal | – | `JST EH2P ST90` ×2 | J5 + J9 |
| 1 | J7 | Micro-USB B, SMD | USB_Micro-B_Reichelt_MIC-USB-BBU | – | `MIC USB BBU` ×1 | own footprint RPN42Library:USB_Micro-B_Reichelt_MIC-USB-BBU |
| 1 | J9 | JST EH 2-pin, 90° | JST_EH_S2B-EH_1x02_P2.50mm_Horizontal | – | – | counted with J5 |
| 1 | J10 | Pin header 1×2 + jumper | PinHeader_1x02_P2.54mm_Vertical | – | `JUMPER 2,54 SW` ×1 | header from the J1 strip |
| 1 | L1 | Ferrite 600 Ω, 0.5 A, 0603 | L_0603_1608Metric_Pad1.05x0.95mm_HandSolder | – | `BLM18AG601SN1D` ×1 |  |
| 2 | L2, L3 | Ferrite 600 Ω, 1.3 A, 0603 | L_0603_1608Metric_Pad1.05x0.95mm_HandSolder | – | `BLM18KG601SN1D` ×2 |  |
| 1 | L4 | 6.3 µH power inductor, Isat 3.8 A, shielded, 6.2 × 6.2 × 4.5 mm (Fastron 242418FPS-6R3N-01) | L_Bourns_SRP5030T | – | `L-242418FPS 6,3µ` ×1 | on the SRP5030 pads (part is a bit larger than the footprint). Irms 3.6 A, ≤ 39 mΩ; 6.3 µH is fine for the TS1935 (ripple ≈ 0.15 A). Other options: Bourns SRP5030T-100M (exact footprint, not at Reichelt), Reichelt `BOU SRN4018-4R7M` (4.7 µH, Isat 2.0 A, 4 × 4 mm) |
| 1 DNP | L5 | alt. 10 µH footprint 10 × 10 mm (Bourns SRU1028/SRU1048) | L_Bourns-SRU1028_10.0x10.0mm | SRU1048-100Y (15, Box AMP2) | – | DNP – alternative to L4, parallel footprint; fit **either** L4 **or** L5 with the stock SRU1048-100Y |
| 1 | Q1 | IRLML6401 P-FET | SOT-23 | – | `IRLML 6401` ×1 | stock DMP3099L: Vth too high for 3 V |
| 1 | Q2 | 2N7002 N-FET | SOT-23 | – | `2N7002` ×1 |  |
| 4 | R1, R5, R23, R24 | 10 k 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | RC0603FR-0710KL (11, Box SMD Reste)<br>ERJ-3EKF1002V (2, Box SMD Reste) | stock · re-order: `SMD-0603 10K` |  |
| 1 | R2 | 470 Ω 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | RC0603FR-07470RL (12, Box SMD Reste) | stock · re-order: `SMD-0603 470` | was 330 Ω, LED data series R |
| 2 | R3, R4 | 2.2 k 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF2201V (14, Box SMD Reste)<br>RC0603FR-072K2L (6, Box SMD Reste) | stock · re-order: `SMD-0603 2,2K` | I²C pull-ups |
| 1 | R13 | 66.5 k 1 % 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF6652V (4, Box SMD Reste) | stock · re-order: `SMD-0603 68K` | boost FB: 1.238 V × (1 + 66.5/22) = 4.98 V |
| 1 | R14 | 22 k 1 % 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF2202V (4, Box SMD Reste) | stock · re-order: `SMD-0603 22K` | boost FB |
| 3 | R15, R20, R21 | 100 k 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF1003V (24, Box SMD Reste)<br>RC0603FR-07100KL (4, Box SMD Reste) | stock · re-order: `SMD-0603 100K` |  |
| 2 | R16, R17 | 200 k 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF2003V (26, Box SMD Reste) | stock · re-order: `SMD-0603 220K` | BAT_SENSE divider (V_bat/2, 10.5 µA) |
| 1 | R22 | 150 k 0603 | R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | ERJ-3EKF1503V (7, Box SMD Reste) | stock · re-order: `SMD-0603 150K` | VBUS_DET divider |
| 2 | SW1, SW2 | Tact switch 6 × 6 × 4.3 mm SMD | SW_SPST_PTS645Sx43SMTR92 | – | `DIP DTSM-61K` ×2 | RESET / BOOT0 |
| 37 | SW38–SW46, SW48–SW58, SW60–SW65, SW67–SW72, SW74–SW78 | Kailh Choc V1 switch | SW_Kailh_Choc_V1_1.00u / 1.25u / 2.00u | own stock (not in Part-DB) (37, –) | – | 16 × 1u, 20 × 1.25u, 1 × 2u positions – same switch, only the footprint spacing differs |
| 1 | U2 | STM32F405RG, LQFP-64 | LQFP-64_10x10mm_P0.5mm | STM32F405RGT7 (1, Ikea Schubbox) | stock · re-order: `STM32F405RGT6` | RGT7 = 105 °C version, fine |
| 1 | U3 | 74AHCT1G125, SOT-353 | SOT-353_SC-70-5 | – | `74AHC T1G125GW` ×1 |  |
| 1 | U4 | MCP1727-3302E/SN LDO, SO-8 | SOIC-8_3.9x4.9mm_P1.27mm | – | `MCP17273302ESN` ×1 |  |
| 1 | U5 | TS1935BCX5 boost, SOT-23-5 | SOT-23-5 | – | `TS1935BCX5` ×1 | stock XL6007 needs ≥ 3.6 V in |
| 1 | U6 | USBLC6-2SC6, SOT-23-6 | SOT-23-6 | – | `USBLC6-2SC6` ×1 | Reichelt: limited quantity |
| 1 DNP | Y1 | alt. 8 MHz crystal HC49-SMD | Crystal_SMD_HC49-SD | – | – | DNP – Y3 is fitted instead (Reichelt 8,0000-HC49-SMD, CL 10 pF, if ever needed) |
| 1 | Y2 | 32.768 kHz, 3.2 × 1.5 mm, CL 6–7 pF | Crystal_SMD_3215-2Pin_3.2x1.5mm | – | elsewhere | **not at Reichelt in 6–7 pF** – order elsewhere, **or** Reichelt `32,768 CC7V-9` (3.2 × 1.5 mm, CL 9 pF, same footprint) with C32/C33 → 12 pF, **or** Reichelt IQD LFXTAL003000 (85SMX, CL 12.5 pF) on Y4 with C32/C33 = 18 pF from stock (less start-up margin on the F405 LSE) |
| 1 | Y3 | 8 MHz crystal, ceramic 5.2 × 3.4 mm, CL 12 pF | Crystal_SMD_0603-4Pin_6.0x3.5mm_HandSoldering | – | `IC14M8` ×1 | fits the 6.0 × 3.5 hand-solder pads; pins 1/3 crystal, 2/4 GND; C1/C2 18 pF match CL 12 pF |
| 1 DNP | Y4 | alt. 32.768 kHz footprint (IQD 85SMX) | Crystal_SMD_IQD_85SMX-4Pin_8.7x3.7mm | – | – | DNP – see Y2 |

## Off-board / mechanics

| Item | Stock (Part-DB) | Note |
|---|---|---|
| TP4056 + DW01 1S charger module | TP4056 1S lipo charger, verschiedene (12, Kiste Solar) | pick one **with** DW01 protection; R_PROG 2.4 k ≈ 500 mA |
| 1S LiPo cell | – | |
| M3 hex standoffs (board + OLED) | Distanzhülsen M3, 5–30 mm (many, Box Distanzhülsen) | holes are 3.7 mm |
| Choc keycaps 16 × 1u, 20 × 1.25u, 1 × 2u | – | 3D printed (`KeycapsForKailhChoc-files/`) |
| Wire strap over J7 | – | 0.6–0.8 mm tinned copper through the two 1.0 mm GND vias |

