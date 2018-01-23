/***************************************************************************************************/
/* 
  Example for 12-bit MAX6675 K-Thermocouple to Digital Converter with Cold Junction Compensation

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/MAX6675

  This sensor uses SPI bus to communicate, specials pins are required to interface

  Connect chip to pins:    SCLK        MISO        don't use SS for CS  don't use MOSI for CS
  Uno, Mini, Pro:          13          12          10                   11
  Mega2560, Due:           52          50          53                   51
  Leonardo, ProMicro:      15          14          x                    16
  NodeMCU 1.0:             GPIO14/D5   GPIO12/D6   GPIO15/D8            GPIO13/D7
  WeMos D1 Mini:           GPIO14/D5   GPIO12/D6   GPIO15/D8            GPIO13/D7

  - K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
  - Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
    with 0.25°C resolution/increment.
  - Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
    Optimal performance of cold junction compensation happends when the thermocouple cold junction
    & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
    near the converter because this may produce an errors.
  - It is strongly recommended to add a 10nF/0.01mF ceramic surface-mount capacitor, placed across
    the T+ and T- pins, to filter noise on the thermocouple lines.

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <MAX31855.h>

uint16_t rawData_01  = 0;
uint16_t rawData_02  = 0;

/*
- for harware spi

  MAX31855(cs)

  cs  - chip select, if CS low serial interface is enabled
*/
MAX31855 myMAX31855_01(3);
MAX31855 myMAX31855_02(4);

void setup()
{
  Serial.begin(115200);

  /* start MAX31855 */
  myMAX31855_01.begin();
  myMAX31855_02.begin();

  while (myMAX31855_01.getChipID() == 0)
  {
    Serial.println("MAX6675_01 error");
    delay(5000);
  }
  Serial.println("MAX6675_01 OK");
}

void loop()
{
  while (myMAX31855_01.detectThermocouple() != MAX31855_THERMOCOUPLE_OK)
  {
    switch (myMAX31855_01.detectThermocouple())
    {
      case MAX31855_THERMOCOUPLE_SHORT_TO_VCC:
        Serial.println("Thermocouple_01 short to VCC");
        break;
      case MAX31855_THERMOCOUPLE_SHORT_TO_GND:
        Serial.println("Thermocouple_01 short to GND");
        break;
      case MAX31855_THERMOCOUPLE_NOT_CONNECTED:
        Serial.println("Thermocouple_01 not connected");
        break;
      case MAX31855_THERMOCOUPLE_UNKNOWN:
        Serial.println("Thermocouple_01 unknown error, check spi cable");
        break;
    }
    delay(5000);
  }

  rawData_01 = myMAX31855_01.readRawData();
  rawData_02 = myMAX31855_02.readRawData();

  Serial.print("Chip_ID_01: ");
  Serial.println(myMAX31855_01.getChipID(rawData_01));

  Serial.print("Chip_ID_02: ");
  Serial.println(myMAX31855_02.getChipID(rawData_02));

  Serial.print("Cold_Junction_01: ");
  Serial.println(myMAX31855_01.getColdJunctionTemperature(rawData_01));

  Serial.print("Cold_Junction_02: ");
  Serial.println(myMAX31855_02.getColdJunctionTemperature(rawData_02));

  Serial.print("Thermocouple_01: ");
  Serial.println(myMAX31855_01.getTemperature(rawData_01));

  Serial.print("Thermocouple_02: ");
  Serial.println(myMAX31855_02.getTemperature(rawData_02));

  delay(5000);
}
