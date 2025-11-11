This is a Emulator for a HP42s calculator.

### Targets

Native - runs on a desktop as a terminal app using ncurses.

STM32 and Teensy - Run on a microcontroller. So far it also uses the Terminal for IO, keystrokes are received with Serial.read and printfs are passed to Serial.print somewhere.

### Platform specifics..

On STM32 or Teensy 
* printf does not do ```%g``` or ```%G``` or ```%f```
  * workaround
  ```
    char szF[20] = "";
    dtostrf(3.14, 4, 2, szF);
  ```

### Stuff

I do builds by myself, no need to query for ```pio run``` or so, as it requires the pio venv.
