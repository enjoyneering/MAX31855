[![license-badge][]][license] ![version]

# MAX31855
This is an Arduino library for 14-bit MAX31855 K-thermocouple to digital converter with 12-bit cold junction compensation & maximum sampling rate about 10Hz. Can work with 5Mhz wardware & software/bit-bang 3-wire SPI & supports all integrated circuit features.

- Maximum power supply voltage is 3.6v
- K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
- Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
  with 0.25°C resolution/increment.
- Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
  Optimal performance of cold junction compensation happends when the thermocouple cold junction
  & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
  near the converter because this may produce an errors.
- It is strongly recommended to add a 10nF/0.01mF ceramic surface-mount capacitor, placed across
  the T+ and T- pins, to filter noise on the thermocouple lines.

Supports:

- Arduino AVR
- Arduino ESP8266
- Arduino ESP32
- Arduino STM32

Beware of a fake MAX31855 K-Thermocouple Sensor Module - http://forum.arduino.cc/index.php?topic=526439.0

[license]:       https://choosealicense.com/licenses/gpl-3.0/
[license-badge]: https://img.shields.io/aur/license/yaourt.svg
[version]:       https://img.shields.io/badge/Version-1.1.0-green.svg
