# Led-Com
## LED controlled proxy built on ESP32
Allows to control ws2812 LED strip connected to the PCs USB/COM port with high level languages.

This application could be built with [esp-idf](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html).
Code is developed with esp-idf 5 and esp32-c3 but should also work with other esp32 versions.
Code is based on [led_strip](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/rmt/led_strip) example
As a LED strip data pin GPIO_10 is use, you can change it in the [LedService.h](main/services/led/LedService.h) file. 

## AT Commands

Set LED strip length. Could be sent only once ofter power on.
```
AT+LN=...uint16 2 bytes little endian...
```

Set LED strip values in format of 3 bytes per color.
```
AT+RGB=...byte stream...
```

## Available API Libraries:
- [NodeJS client](https://github.com/s256v/led-com-node)
- Java client (not implemented yet)