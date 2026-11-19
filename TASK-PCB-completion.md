# PCB completion

This is becoming a PCB for a slightly oversized RPN calculator.

## Schematics sanity check

OLED and Key placement are done to fit the purpose.

### MCU

..design is coarse - check that the stm32f405 has everything it needs.
There is a somewhat complete schematic in  ../kicad-mcp-test/test02/

### LiPo battery

There is a Bat charger in ./HeadPhoneAmp/pcb-kicad/PowerBoost5V12V
Maybe copy them schematic into a new subcircuit of this project ?

### LEDs

All the choc buttons should have an LED in the cutout.
Extend the schematic to cover the lower key rows as well.

## PCB

layout.
keep most stuff around the top...

## Refs

drawings of the switches and the OLED are under ./images

---

# Notes (schematic done 2026-11-19)

KiCad project: `PCB-KiCAD/TeensyRPN42/`. Sheets: root (MCU, OLED, SWD/serial, LDO),
`untitled.kicad_sch` (key matrix), `leds.kicad_sch` (SK6805 chain), `power.kicad_sch` (Micro-USB, charger, boost).
Fixed mechanics (OLED, switches, M3 holes, outline): see `CLAUDE.md` and `check_fixed.py`.

## MCU pin usage (STM32F405RGT6, LQFP64)

| Function | Pin(s) | Notes |
|---|---|---|
| Key matrix ROW1 / ROW2 | PA9 / PA10 | row = output, drive **only the active row low**, others hi-Z |
| Key matrix ROW3–ROW6 | PB12–PB15 | |
| Key matrix ROW7 | PA15 | |
| Key matrix COL0–COL5 | PC7–PC12 | inputs with pull-up |
| OLED SPI1 | PA5 SCK, PA7 MOSI, PA4 CS, PC4 D/C | RESET = NRST (shared). PA6/MISO1 unused |
| LED data (SK6805 chain) | PB10 | → 74AHCT125 (5 V level shift) → 330 Ω → D1 … D38 |
| I²C1 (J2, mini OLED / spare) | PB8 SCL, PB9 SDA | 2.2 k pull-ups on board |
| USART2 (J4) | PA2 TX, PA3 RX | |
| USB FS | PA11 D−, PA12 D+ | Micro-USB J7, USBLC6 ESD (U6 pins 4/6 MCU side). No VBUS sense on PA9 (used as ROW1) → disable VBUS sensing |
| SWD (J1) | PA13 SWDIO, PA14 SWCLK, PB3 SWO, NRST | J1.1 = 3V3 (VTref) |
| Charger STAT1 / STAT2 / PG | PC0 / PC1 / PC2 | open drain. STAT1/2 also drive CHG/DONE LEDs from VBUS → read without pull-up; PG needs the internal pull-up |
| Boost enable | PC13 | high = 5 V on (LEDs + 74AHCT125). 100 k pull-down → off in reset |
| Battery voltage | PA0 (ADC1_IN0) | 1 M / 1 M divider → V_bat / 2 |
| LSE 32.768 kHz | PC14 / PC15 | RTC (Free42 clock functions). 6.8 pF caps, crystal CL ≈ 6 pF |
| HSE 8 MHz | PH0 / PH1 | Y1 (HC49). Y3 is an alternative footprint, DNP |
| Spare (labelled `SPARE_*`, not routed) | PA1, PA8, PB0, PB1, PB2, PB4–PB7, PB11, PC3, PC5, PC6, PD2 | |

### Key matrix (rows × columns)

| Row | Pin | Switches (left → right) | Columns |
|---|---|---|---|
| 1 | PA9  | SW38 SW45 SW51 SW58 SW65 SW68 | 0 1 2 3 4 5 |
| 2 | PA10 | SW39 SW40 SW41 SW42 SW43 SW44 | 0 1 2 3 4 5 |
| 3 | PB12 | SW46 (ENTER 2u) SW48 SW49 SW50 SW52 | 0 2 3 4 5 |
| 4 | PB13 | SW53 SW54 SW55 SW56 SW57 | 0 1 2 4 5 |
| 5 | PB14 | SW60 SW61 SW62 SW63 SW64 | 0 1 2 4 5 |
| 6 | PB15 | SW67 SW69 SW70 SW71 SW72 | 0 1 2 4 5 |
| 7 | PA15 | SW74 SW75 SW76 SW77 SW78 | 0 1 2 4 5 |

No diodes: fine for one key at a time (calculator UI is sequential). Ghosting needs ≥ 3 held keys.

### LED chain

D1–D6 row 1, D7–D12 row 2, D13–D18 row 3 (D13 + D14 both under the 2u ENTER), D19–D38 rows 4–7,
each row left → right. 38 × SK6805 at full white is > 1 A → **limit brightness in firmware** (boost ≈ 1 A).
LEDs only work while BOOST_EN (PC13) is high.

## Power

Micro-USB J7 (Molex 105017-0001, top-mount, ID pin NC) → `VBUS_IN` → F1 polyfuse 750 mA → `VBUS`
(D41 SMAJ5.0A TVS on `VBUS_IN`) → MCP73871-2CC (U1, charger + power path) → `PWR_SYS`.
USB data via U6 USBLC6 (connector side pins 1/3, MCU side 4/6).
- Input limited to **500 mA** (SEL = GND → USB mode, PROG2 = VBUS → 500 mA): safe on PC ports and weak
  chargers; with LEDs on the battery supplements.
- Charge current 500 mA (PROG1 = 2 k), termination 50 mA (PROG3 = 20 k), safety timer on (/TE = GND),
  VPCC ≈ 4.5 V (270 k / 100 k), THERM = fixed 10 k (no NTC). Battery on J5 (JST-EH, pin 2 = +, pin 1 = GND —
  check the pack's polarity!).
- `PWR_SYS` → MCP1727 LDO (U4) → +3.3 V (MCU, OLED). Works from USB or battery, boost not needed.
- `PWR_SYS` → TPS61030 (U5, boost, 4.7 µH) → +5 V (LEDs via ferrite L3, 74AHCT125). FB 1.8 M / 200 k = 5.0 V.
- Charge LEDs (only lit with USB plugged in): D39 red = charging (STAT1), D40 green = done (STAT2).
- Soft-off: firmware drops BOOST_EN, puts OLED to sleep and the MCU into STOP; ON key (matrix) wakes it.

## PCB routing status (2026-11-19)

Done (scripted / small grid router, DRC-clean except the known items below):
- Key matrix spines: ROW lines on F.Cu straight through pin 1 of each row, COL spines on B.Cu 2.6 mm left of
  pin 2 (doglegs at y 150 / 171.3). **Not yet connected to the MCU.** Hand-off points: COL spines leave row 1
  at x ≈ 49.1 / 67.8 / 86.6 / 105.2 / 124.0 / 142.7 (y 123.8); ROW lines end at the outer pin-1 pads of each row.
- LED chain D1…D38 complete (data, row-to-row links), V-LED buses per row, left trunk (x 48.77) and right trunk
  (x 159.0, rows 1 and 4–7, fed by L3 at the right edge).
- Power cluster: Micro-USB J7 → F1/D41 → VBUS, USB D± J7 → U6, charger U1 (PROG/VPCC/THERM, +BATT, PWR_SYS), boost U5 (L4, FB, caps, +5V),
  LDO input, charge LEDs, V-OLED filter. U1/U5 use plain QFN footprints + 4 × 0.7/0.3 thermal vias each.
- GND: vias next to the SMD GND pads (R8/R9 still without), stitching grid only in the key area (y > 113).

To do (by hand, KiCad interactive router):
- MCU fan-out: USB D+/D− (from U6), crystals (Y1/Y3, Y2/Y4 + caps), VCAP1/2, VDDA, +3.3V (LDO → MCU + caps,
  R3/R4/R5, J1, L1, L2), NRST, BOOT0, SWD (TCK/TMS/SWO), USART, SPI/OLED (SCK, MOSI, CS, D/C), I²C (SCL/SDA → J2),
  LEDs (PB10 → U3), CHG_STAT1/2, CHG_PG, BOOST_EN, BAT_SENSE, all ROW/COL to the MCU.
  Fan-out under the LQFP body (stub + via) is fine; the caps sit close to the pins on the right side.
- Afterwards: GND stitching in the top area, fix starved thermals, silkscreen, finishing touches.
- Known DRC items: LED-in-switch courtyard overlaps (intended), J3 keepout flags J3's own pads (rule area
  should not forbid pads, or exclude J3).

## Programming

- **USB DFU** (no extra tools): hold BOOT0 (SW2, back side), tap RESET (SW1), release BOOT0.
  `dfu-util -a 0 -s 0x08000000:leave -D firmware.bin`
- **SWD** (J1, back side): ST-Link → 3V3 (pin 1), SWCLK (2), GND (3), SWDIO (4), NRST (5), SWO (6).
  PlatformIO: `upload_protocol = stlink`.
- Serial console: J4 (GND, TX, RX) at 3.3 V, 115200 baud.

## Building (firmware)

- PlatformIO env `stm32f4` currently targets `nucleo_f411re`. For this board use
  `board = genericSTM32F405RG` (STM32duino variant `F405RGT_F415RGT`). Its defaults do **not** match this PCB:

  | Item | Variant default | This board | Fix |
  |---|---|---|---|
  | `Serial` | UART4 on PA0/PA1 (PA0 = BAT_SENSE here!) | USART2 PA2 TX / PA3 RX (J4) | `-DSERIAL_UART_INSTANCE=2 -DPIN_SERIAL_TX=PA2 -DPIN_SERIAL_RX=PA3` |
  | `Wire` (I²C1) | PB6/PB7 | PB8 SCL / PB9 SDA (J2) | `-DPIN_WIRE_SCL=PB8 -DPIN_WIRE_SDA=PB9` (or `Wire.setSCL/setSDA`) |
  | `SPI` (SPI1) | PA5/PA6/PA7, SS PA4 | same (OLED: SCK PA5, MOSI PA7, CS PA4, D/C PC4) | none |
  | Clock | HSI → PLL 168 MHz | 8 MHz HSE (Y1/Y3) | own `SystemClock_Config()`: HSE, PLLM 8, PLLN 336, PLLP 2, PLLQ 7 (48 MHz USB). HSI is not accurate enough for USB |

  Optional USB serial instead: `-DPIO_FRAMEWORK_ARDUINO_ENABLE_CDC -DUSBCON` → `Serial` = USB CDC,
  hardware USART2 then via `HardwareSerial Serial2(PA3, PA2)`. USB VBUS sensing must stay off (PA9 = ROW1).
- Keymap: see the matrix table above (row-scan, columns with pull-ups).
