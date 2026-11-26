# What ???

Trying to compile Free42 on a Teensy4 / STM32F4xx.

It's a PlatformIO project, copied some code from free42 into lib and trying to compile it...

Allmost, just some link references to open/unlink/link file operations triggered from somethere ???

# Status

Ultraexperimental.

Works with double precision over terminal.

```
╒══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╕
│                                                                                                                  │
│                                                                                                                  │
│             ▄▀▀▀▄       ▄▀▀▀▄ ▄▀▀▀▄ ▄▀▀▀▄ ▀▀▀▀█  ▄█   ▀▀▀▀█ ▄▀▀▀▄   ▄█  ▀▀▀▀█ ▄▀▀▀▄ ▄▀▀▀▄ ▄▄▄▄         ▄█        │
│ ▀▄ ▄▀ ▄       ▄▄▀       ▀▄▄▄█   ▄▄▀  ▄▄▄▀   ▄▀    █     ▄▀  █ ▄▀█ ▄▀ █    ▄▀    ▄▄▀  ▄▄▄▀ █▄▄   ▄▄▄▄    █        │
│   █   ▄     ▄▀     ▄▄      ▄▀ ▄▀    ▄   █  █      █    █    █▀  █ ▀▀▀█▀  █    ▄▀    ▄   █ █▄▄▄          █        │
│   ▀         ▀▀▀▀▀  ▀▀    ▀▀   ▀▀▀▀▀  ▀▀▀   ▀     ▀▀▀   ▀     ▀▀▀     ▀   ▀    ▀▀▀▀▀  ▀▀▀               ▀▀▀       │
│                    ▄█         ▀▀▀▀█ █▀▀▀▀  ▄▀▀  ▄▀▀▀▄ ▀▀▀▀█ ▄▀▀▀▄ █▀▀▀▀ █▀▀▀▀ ▄▀▀▀▄  ▄█   ▀▀▀▀█ ▄▄▄▄         ▄█  │
│ ▀▄ ▄▀ ▄     ▄▄▄▄    █           ▄▀  ▀▀▀▀▄ █▄▄▄   ▄▄▄▀   ▄▀   ▄▄▄▀ ▀▀▀▀▄ ▀▀▀▀▄ ▀▄▄▄▀   █     ▄▀  █▄▄   ▄▄▄▄    █  │
│  ▄▀▄  ▄             █    ▄▄    █    ▄   █ █   █ ▄   █  █    ▄   █ ▄   █ ▄   █ █   █   █    █    █▄▄▄          █  │
│ ▀   ▀              ▀▀▀   ▀▀    ▀     ▀▀▀   ▀▀▀   ▀▀▀   ▀     ▀▀▀   ▀▀▀   ▀▀▀   ▀▀▀   ▀▀▀   ▀                 ▀▀▀ │
╘══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╛
```

# Software

* replaced scanf %e %g with own parser.

* quad precission ?
  * checking free42(swissmicro build process..)
    * gcc111libbid-arm64.a - lib src is packaged with it. And available on github
    * there is scanf stuff in ```common/core_phloat.cc``` only when not ARM build ?
    * cmd line:
    ```
    arm-none-eabi-g++ -Os -DDM42_OLDHW -I../common -DRELEASE -MMD -Wall -Wno-misleading-indentation -Wno-parentheses -Wno-write-strings -Wno-maybe-uninitialized -Wno-unknown-pragmas -Wno-class-memaccess -Wno-sign-compare -fno-exceptions -fno-rtti -DVERSION="\"3.3.8\"" -DDECIMAL_CALL_BY_REFERENCE=1 -DDECIMAL_GLOBAL_ROUNDING=1 -DDECIMAL_GLOBAL_ROUNDING_ACCESS_FUNCTIONS=1 -DDECIMAL_GLOBAL_EXCEPTION_FLAGS=1 -DDECIMAL_GLOBAL_EXCEPTION_FLAGS_ACCESS_FUNCTIONS=1 -D_WCHAR_T_DEFINED  -fdata-sections -ffunction-sections -Wl,--gc-sections -ggdb -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DARM -DBCD_MATH -c -o core_phloat.o ../common/core_phloat.cc
    ```

  * It builds with the intel lib but it's a bit (way too) large
    ```
    ld: region `RAM' overflowed by 798408 bytes
    ld: region `FLASH' overflowed by 2006716 bytes
    ```

# Hardware

Ideas...

* STM32 ? F411RE - is dev board (Or F429? 82-168 IOs)
  ```
  RAM:   [=         ]   9.7% (used 12704 bytes from 131072 bytes)
  Flash: [======    ]  62.7% (used 328636 bytes from 524288 bytes)
  ```
  Connect to Nucleo STLink programmer part:
  * Remove SB13/SB14 for TX/RX connection to Nucleo MCU
  * Connect SWD (SWO not needed) (Open Jumpers to Nucleo MCU)
  * Connect TX->RX Rx->TX

* Display ???
  * OLED ? 256x64
* Keys 
  * K.Choc low profile mechaniks
* Power ?
  * Single Lipo ? Cheap Charger...


## PCB Ordered 

commit: 9db91b7e23bbdf1880c24b6f514a0bee10d9d54c

## Costa ?

| What | Where | $$$ |
|---|---|---:|
| PCB | JLC 5x | 27/5 = 5.4 |
| OLED "OLED Display 2.8" 256*64" | AliEx | 22 |
| Keith Choc V1 | AliEx | ~30 |
| LEDs "WS2812 2020" 500x | AliEx | 22/13 = 1.7 |
| BOM all | Reichelt | 20 |
| **Sum** | | ~100 |

# Refs

thomasokken's ingenious https://thomasokken.com/free42 https://github.com/aswild/free42

And code on github: https://github.com/swissmicros/free42

OpenRPNCalc ? on github, uses CubeIDE 🤯 and own code..
