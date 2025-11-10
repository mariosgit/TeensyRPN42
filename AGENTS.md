This is a Emulator for a HP42s calculator.

### Targets

Native - runs on a desktop as a terminal app using ncurses.

STM32 and Teensy - Run on a microcontroller. So far it also uses the Terminal for IO, keystrokes are received with Serial.read and printfs are passed to Serial.print somewhere.
