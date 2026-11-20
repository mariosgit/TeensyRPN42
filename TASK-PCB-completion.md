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
| USB present (VBUS_DET) | PC2 | VBUS via 100 k / 150 k → ≈ 3.0 V when USB is plugged in, 0 V otherwise (no pull needed) |
| Boost enable | PC13 | high = 5 V on (LEDs + 74AHCT125) via Q2/Q1 load switch. 100 k pull-down → off in reset |
| Battery voltage | PA0 (ADC1_IN0) | 1 M / 1 M divider on PWR_SYS (= module OUT+ = battery) → V_bat / 2 |
| LSE 32.768 kHz | PC14 / PC15 | RTC (Free42 clock functions). 6.8 pF caps, crystal CL ≈ 6 pF |
| HSE 8 MHz | PH0 / PH1 | Y1 (HC49). Y3 is an alternative footprint, DNP |
| Spare (labelled `SPARE_*`, not routed) | PA1, PA8, PB0, PB1, PB2, PB4–PB7, PB11, PC0, PC1, PC3, PC5, PC6, PD2 | PC0/PC1 were charger STAT1/2 |

All the STM32F405's "V…" supply pins need decoupling, and the schematic already provides it. Only the placement needs a look: C28 is far from its pin, and the two VDDA caps are a bit far.

| Pin (LQFP64) | Net | Caps in the schematic | Required | On the PCB now |
|---|---|---|---|---|
| **31 VCAP_1** | /MCU_VCAP1 | C27 2.2 µF | **Required.** Without it the internal 1.2 V core regulator doesn't start. Low-ESR ceramic to GND | C27 at (83.6, 84.0), about 5 mm from pin 31 ✔ |
| **47 VCAP_2** | /MCU_VCAP2 | C28 2.2 µF | **Required**, same as VCAP_1 | ⚠ C28 at (99.4, 109.7), about **21 mm** from pin 47 (82.2, 98.2). Needs to move next to pin 47 on the left side of U2 |
| **13 VDDA** | /VDDA (via L1 from +3.3V) | C6 100 nF + C30 1 µF | Recommended. Analog/ADC supply; returns through 12 VSSA | C6/C30 at y ≈ 84, about 9 mm from pin 13 (93.5, 92.7). Closer is better |
| **1 VBAT** | +3.3V | C29 100 nF | Recommended (powers the RTC/LSE domain) | C29 at (93.4, 104.4), about 6 mm ✔ |
| **19, 32, 48, 64 VDD** | +3.3V | one 100 nF each, plus C31 4.7 µF bulk | 100 nF per VDD pin, plus one 4.7 µF bulk | 19 → C10, 32 → C8, 48 → C9, 64 → C7/C12, all about 4–6 mm. C31 bulk at the bottom ✔ |

There are 6 × 100 nF on +3.3V for 4 VDD pins, so C11 (at 87.4, 83.9) is a spare and could go near VBAT or VDD 19. C5 10 µF is at the LDO output, not at the MCU, which is fine.

The ground pins 12 VSSA, 18 VSS and 63 VSS need short paths to GND (vias right at the pins), but no caps of their own.

Pin 30 is PB11 (a spare). Some smaller F4 parts put VCAP there, but the F405 has VCAP only on pins 31 and 47.


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

Changed 2026-11-20: the on-board MCP73871 charger and TPS61030 boost were replaced by an **off-board TP4056
charger module (with DW01 protection)** and an **XL6007 boost (SOP-8)**. No power path (kept simple on purpose).

```
J7 µUSB → F1 750 mA → VBUS ─┬─ U6 USBLC6 (data), C34, R21/R22 → VBUS_DET (PC2)
 (D41 TVS on VBUS_IN)        └─ J9 "CHG IN" (JST-EH: 1 GND, 2 VBUS) ──cable──► module IN+ / IN−
battery ──► module B+ / B−   (battery never connects to our board directly)
module OUT+ / OUT− ──cable──► J5 "BAT" (JST-EH: 1 GND, 2 PWR_SYS)
PWR_SYS ─┬─ U4 MCP1727-3302E/SN → +3.3 V (MCU, OLED), PWRGD → NRST
         ├─ R16/R17 → BAT_SENSE (PA0)
         └─ Q1 IRLML6401 (load switch) → BOOST_IN → L4 10 µH → SW → D42 SS34 → +5 V (C38/C39 22 µF, C54 220 µF)
              gate: R20 100 k to PWR_SYS, pulled low by Q2 2N7002 when BOOST_EN (PC13) = high
            U5 XL6007: VIN = EN = BOOST_IN, FB = R13 30 k / R14 10 k → 1.25 V × 4 = 5.0 V
```
- **Module wiring:** our GND only to the module's IN−/OUT− (never B−, that is the switched battery minus).
- **Module PROG resistor:** stock modules charge at 1 A (R_PROG 1.2 k, often "R3"/"122"). Change it to
  **2.4 k (≈ 500 mA)** or higher (3 k ≈ 400 mA) — otherwise charge current + board load exceed F1 (750 mA)
  and a PC USB port. Charge LEDs are on the module.
- **Why the Q1 load switch:** the XL6007 is a non-synchronous boost; with EN low the battery would still reach
  +5 V through L4 + D42 (≈ 3.5 V) and the 38 SK6805 would draw their idle current. Q1 cuts the boost input → 0 mA
  in soft-off.
- **XL6007 limits:** VIN ≥ 3.6 V (LEDs go dark below that, MCU/OLED keep running from the LDO), 2 A switch
  limit → ≈ 1 A at 5 V from 3.6 V. L4 needs Isat ≥ 3 A (e.g. SRP5030T-100M, check Isat), D42 3 A Schottky.
- No power path: while USB is plugged in the board runs from the module's OUT+ (charger + battery in
  parallel). With a large load the TP4056 may not terminate cleanly — keep LEDs dim/off while charging.
- **Firmware rules:** LEDs off below ≈ 3.65 V (XL6007 UVLO), warning/soft-off at ≈ 3.4 V (DW01 only cuts at
  ≈ 2.5 V), total LED current < ≈ 0.8 A, read VBUS_DET (PC2) for "charging".
- Soft-off: firmware drops BOOST_EN, puts OLED to sleep and the MCU into STOP; ON key (matrix) wakes it.

## PCB routing status (2026-11-19)

Done (scripted / small grid router, DRC-clean except the known items below):
- Key matrix spines: ROW lines on F.Cu straight through pin 1 of each row, COL spines on B.Cu 2.6 mm left of
  pin 2 (doglegs at y 150 / 171.3). **Not yet connected to the MCU.** Hand-off points: COL spines leave row 1
  at x ≈ 49.1 / 67.8 / 86.6 / 105.2 / 124.0 / 142.7 (y 123.8); ROW lines end at the outer pin-1 pads of each row.
- LED chain D1…D38 complete (data, row-to-row links), V-LED buses per row, left trunk (x 48.77) and right trunk
  (x 159.0, rows 1 and 4–7, fed by L3 at the right edge).
- Power cluster: **re-placed 2026-11-20 for TP4056 module + XL6007, not routed** (user routes by hand):
  J5 BAT + J9 CHG IN side by side at the top edge (x 116.9 / 125.5), Q2/Q1/R20/R15 load switch above L4,
  U5 (SOIC-8, rot 270) right of L4, D42 + C38/C39/C54 (+5V) below, C37/C55 at U5 VIN, R13/R14 (FB) right,
  R21/R22 (VBUS_DET) next to C34. Old charger/boost copper removed; still-valid links (BAT_SENSE, LDO) kept.
- GND: vias next to the SMD GND pads (new power parts still without), stitching grid only in the key area (y > 113).

To do (by hand, KiCad interactive router):
- MCU fan-out: USB D+/D− (from U6), crystals (Y1/Y3, Y2/Y4 + caps), VCAP1/2, VDDA, +3.3V (LDO → MCU + caps,
  R3/R4/R5, J1, L1, L2), NRST, BOOT0, SWD (TCK/TMS/SWO), USART, SPI/OLED (SCK, MOSI, CS, D/C), I²C (SCL/SDA → J2),
  LEDs (PB10 → U3), VBUS_DET (PC2), BOOST_EN, BAT_SENSE, all ROW/COL to the MCU.
  Power: VBUS (F1 → C34 → J9, R21), PWR_SYS (J5 → C35/C4/U4, Q1, R20, R16), boost cluster, +5V → L3 / U3.
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
