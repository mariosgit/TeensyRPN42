# TeensyRPN42 – PCB

Main board for an RPN calculator running [Free42](https://thomasokken.com/free42/) on an STM32F405.
37 Kailh Choc V1 keys, each with an SK6805 RGB LED in the key window, a 2.8" 256×64 SPI OLED, and a LiPo
supply with a switchable 5 V boost for the LEDs. Two layers; all parts on the top side except the SWD (J1) and serial (J4) headers.

| Front | Headers | Back |
|---|---|---|
| ![iso front](renders/teensyrpn42_iso_front.png) | ![iso headers](renders/teensyrpn42_iso_headers.png) | ![iso back](renders/teensyrpn42_iso_back.png) |
| ![top](renders/teensyrpn42_top.png) | | ![bottom](renders/teensyrpn42_bottom.png) |

**Interactive BOM:** [bom/TeensyRPN42_ibom.html](bom/TeensyRPN42_ibom.html) (open in a browser; placement per part, with Reichelt numbers).

## Specs

| | |
|---|---|
| Board | 113.0 × 184.1 mm, 3 mm corner radius, 2 layers, 4 × M3 mounting holes (3.7 mm, tolerance for the OLED module) |
| Fab | AISLER 2-layer simple (min. track 0.2 mm, clearance 0.15 mm, via 0.7/0.3 mm), vias tented |
| MCU | STM32F405RGT6 (LQFP64), 8 MHz HSE (Y3, ceramic 5.2 × 3.4 mm), 32.768 kHz LSE (Y2) for the RTC |
| Display | 2.8" OLED module, 16-pin header J3, SPI1 |
| Keys | 37 × Choc V1 (1u / 1.25u / 2u ENTER), 7 × 6 matrix, no diodes |
| LEDs | 38 × SK6805 2020 (2 × 2 mm, on the 2427 footprint; two under the 2u ENTER), one data chain, 5 V via 74AHCT125 level shifter |
| Power | 1-cell LiPo through an off-board TP4056 + DW01 module (J5 BAT, J9 CHG Vusb) |
| 3.3 V | MCP1727-3302 LDO from the battery (MCU, OLED) |
| 5 V | TS1935BCX5 boost (2.6–5.5 V in, 1.9 A switch), switched by PC3 through a P-FET load switch |
| USB | Micro-USB (J7), USB FS on PA11/PA12, USBLC6 ESD, 750 mA PTC, SMAJ5.0A TVS |
| Programming | USB DFU (BOOT0 button / jumper), SWD (J1) |

## Connectors

| Ref | Side | Pins |
|---|---|---|
| J1 SWD | back | 1 3V3 (VTref) · 2 SWCLK · 3 GND · 4 SWDIO · 5 NRST · 6 SWO |
| J4 Serial | back | GND · TX (PA2) · RX (PA3), 3.3 V, 115200 baud |
| J5 BAT | top edge | 1 GND · 2 PWR_SYS ← TP4056 module OUT+/OUT− (never B−) |
| J9 CHG Vusb | top edge | 1 GND · 2 VBUS → TP4056 module IN+/IN− |
| J10 BOOT0 | top | jumper = BOOT0 to 3.3 V → every reset lands in DFU |
| J2 / J8 | top | I²C1: PB8 SCL, PB9 SDA, 2.2 k pull-ups (mini OLED / spare) |
| J3 | top | 2.8" OLED module (SPI1) |
| J7 | top edge | Micro-USB (data + charging) |
| SW1 / SW2 | top (right / left edge) | RESET / BOOT0 |

## MCU pin map

| Function | Pins |
|---|---|
| Matrix rows 1…7 | PA9, PA10, PD2, PC12, PC11, PC10, PA15 |
| Matrix columns 0…5 | PC7, PC8, PC9, PB5, PB6, PB7 (inputs, pull-up) |
| OLED (SPI1) | PA5 SCK, PA7 MOSI, PA4 CS, PC4 D/C, RESET = NRST |
| LED data | PB10 → 74AHCT125 → 470 Ω → D1 … D38 |
| I²C1 | PB8 SCL, PB9 SDA |
| USART2 | PA2 TX, PA3 RX |
| USB FS | PA11 D−, PA12 D+ (VBUS sensing off, PA9 is ROW1) |
| SWD | PA13 SWDIO, PA14 SWCLK, PB3 SWO |
| VBUS_DET | PA1, 100 k / 150 k divider → ≈ 3.0 V with USB |
| BAT_SENSE | PA0, 200 k / 200 k divider → V_bat / 2 |
| BOOST_EN | PC3, high = 5 V on (100 k pull-down, off during reset) |
| BOOT0 / BOOT1 | BOOT0 10 k pull-down (SW2 / J10 → 3.3 V), PB2 10 k pull-down |
| Spare | PA8, PB0, PB1, PB4, PB11, PB12–PB15, PC0, PC1, PC2, PC5, PC6, PC13 |

## BOM

Full BOM with your Part-DB stock and the Reichelt order list: **[bom/BOM.md](bom/BOM.md)**.

- Resistors, 100 nF, 1 nF, 18 pF, the STM32F405 and the OLED come from stock.
- Reichelt: 22 positions, about 8.60 € (ICs, FETs, ferrites, 22 µF, PTC, TVS, Schottky, crystal Y3 (IC14M8), DTSM-61K, MIC USB BBU, headers).
- L4: Reichelt `L-242418FPS 6,3µ` (6.3 µH, Isat 3.8 A) on the SRP5030 pads; L5 (DNP) is a parallel 10 × 10 mm footprint for the stock SRU1048-100Y.
- Elsewhere: Y2 (32.768 kHz 3215, CL 6–7 pF), 38 SK6805 2020 LEDs (ordered).
- Own stock: 37 Kailh Choc V1 switches; keycaps are 3D printed (`KeycapsForKailhChoc-files/`).

**DNP (footprint only):** L5 (10 × 10 mm alternative to L4), Y1 (alternative 8 MHz HC49 footprint),
Y4 (alternative 32.768 kHz footprint, fits the Reichelt IQD LFXTAL003000).
**Off-board:** TP4056 + DW01 charger module (set R_PROG to 2.4 k ≈ 500 mA), 1-cell LiPo.

## Design notes

- **Power path:** battery → module OUT+ → J5 → PWR_SYS. PWR_SYS feeds the LDO (always on) and the boost
  through Q1. BOOST_EN (PC3) high → Q2 pulls Q1's gate low → BOOST_IN = battery → U5 starts → +5 V.
  R24 10 k + C56 100 nF slow Q1's turn-on to ≈ 0.9 ms (inrush into the 5 V / LED caps).
  Q1 is needed because a non-synchronous boost always conducts battery → L4 → D42 → +5 V; without it the
  LEDs would draw idle current in soft-off.
- **5 V budget:** ≈ 0.6 A at 5 V from a 3.0 V battery. Limit LED brightness in firmware (total LED current
  ≲ 0.5 A). Soft-off / low-battery warning at ≈ 3.4 V (BAT_SENSE); the DW01 only cuts at ≈ 2.5 V.
- **V-LED:** separate branch behind ferrite L3 with C53 22 µF at the feed point and 24 × 100 nF spread along the rows.
  Ladder layout: one trunk on each side, one bus per key row, so most rows are fed from both ends.
- **Filtering:** L1 + C6/C30 for VDDA, L2 + C13 for the OLED supply.
- **Key matrix (no diodes):** drive only the active row low, all other rows hi-Z; columns are inputs with
  pull-ups. One or two keys at a time are reliable; ghosting needs three or more held keys.
- **DFU:** hold BOOT0 (SW2), tap RESET (SW1), release. During development, a jumper on J10 keeps the board
  in DFU after every reset. `dfu-util -a 0 -s 0x08000000:leave -D firmware.bin`.
- **Clock:** use HSE 8 MHz → PLL 168 MHz (PLLM 8, PLLN 336, PLLP 2, PLLQ 7 = 48 MHz USB); HSI is not
  accurate enough for USB.
- **Fixed mechanics:** OLED J3, the 37 switches, H1–H4 and the outline are locked;
  `python3 check_fixed.py` verifies them against `fixed_placement.json`.
- **DRC:** clean. The LED-in-switch courtyard overlaps (intended) and H3/H4 vs J3 are excluded. Footprint
  library-mismatch warnings are local edits (silk tweaks) and intentional.
