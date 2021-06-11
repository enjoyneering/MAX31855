[![license-badge][]][license] ![version] [![stars][]][stargazers] ![hit-count] [![github-issues][]][issues]

# MAX31855
This is an Arduino library for 14-bit MAX31855 K-thermocouple to digital converter with 12-bit cold junction compensation & maximum sampling rate about 10Hz. Supports all sensor features. Can work with hardware or software emulated 3-wire SPI. 

- Maximum power supply voltage is 3.6v
- K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
- Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
  with 0.25°C resolution/increment.
- Maximun SPI bus speed 5Mhz
- Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
  Optimal performance of cold junction compensation happends when the thermocouple cold junction
  & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
  near the converter because this may produce an errors.
- It is **STRONGLY** recommended to add a 10nF/0.01mF capacitor, placed across
  the T+ and T- pins, to filter noise/false readings on the thermocouple lines.
  
**Library returns 2000, if thermocouple open/shorted or a communication error has occurred

Supports:

- Arduino AVR
- Arduino ESP8266
- Arduino ESP32
- Arduino STM32

Beware of a [fake MAX31855 K-Thermocouple Sensor Module](http://forum.arduino.cc/index.php?topic=526439.0)

[license-badge]: https://img.shields.io/badge/License-GPLv3-blue.svg
[license]:       https://choosealicense.com/licenses/gpl-3.0/
[version]:       https://img.shields.io/badge/Version-1.2.3-green.svg
[stars]:         https://img.shields.io/github/stars/enjoyneering/MAX31855.svg
[stargazers]:    https://github.com/enjoyneering/MAX31855/stargazers
[hit-count]:     https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fenjoyneering%2FMAX31855&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false
[github-issues]: https://img.shields.io/github/issues/enjoyneering/MAX31855.svg
[issues]:        https://github.com/enjoyneering/MAX31855/issues/
