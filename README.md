# What ???

Trying to compile Free42 on a Teensy4.

It's a PlatformIO project, copied some code from free42 into lib and trying to compile it...

Allmost, just some link references to open/unlink/link file operations triggered from somethere ???

# Status

Ultraexperimental

# Software

Not running on MC ! Input Values are not stored ???

# Hardware

Ideas...

* STM32 ? F411RE - is dev board (Or F429? 82-168 IOs)
  ```
  RAM:   [=         ]   9.7% (used 12704 bytes from 131072 bytes)
  Flash: [======    ]  62.7% (used 328636 bytes from 524288 bytes)
  ```
* Display ???
  * OLED ? 256x64
  * VFD ?? cool but power usage ???
* Keys 
  * K.Choc low profile mechaniks
  * cheap 12mm clickers ?
* Power ?
  * Single Lipo ? Cheap Charger...

# Refs

thomasokken's ingenious https://thomasokken.com/free42 

And code on github: https://github.com/swissmicros/free42

OpenRPNCalc ? on github, uses CubeIDE and own code..
