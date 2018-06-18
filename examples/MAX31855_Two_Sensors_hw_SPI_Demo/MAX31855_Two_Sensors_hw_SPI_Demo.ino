/***************************************************************************************************/
/* 
  Example for 12-bit MAX6675 K-Thermocouple to Digital Converter with Cold Junction Compensation

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/MAX6675

  - MAX31855 maximum power supply voltage is 3.6v
  - K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
  - Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
    with 0.25°C resolution/increment.
  - Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
    Optimal performance of cold junction compensation happends when the thermocouple cold junction
    & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
    near the converter because this may produce an errors.
  - It is strongly recommended to add a 10nF/0.01mF ceramic surface-mount capacitor, placed across
    the T+ and T- pins, to filter noise on the thermocouple lines.

  This sensor uses SPI bus to communicate, specials pins are required to interface
  Board:                                    SCLK        MISO        SS don't use for CS   MOSI
  Uno, Mini, Pro, ATmega168, ATmega328..... 13          12          10                    11
  Mega2560, Due............................ 52          50          53                    51
  Leonardo, ProMicro, ATmega32U4........... 15          14          x                     16
  Blue Pill, STM32F103xxxx boards.......... PA5**       PA6**       PA4**                 PA7**
  NodeMCU 1.0, WeMos D1 Mini............... GPIO14/D5   GPIO12/D6   GPIO15/D8*            GPIO13/D7

                                           *if GPIO2/D4 or GPIO0/D3 used for for CS, apply an external 25kOhm
                                            pullup-down resistor
                                          **STM32F103xxxx pins are NOT 5v tolerant, bi-directional
                                            logic level converter is needed

  Frameworks & Libraries:
  ATtiny Core           - https://github.com/SpenceKonde/ATTinyCore
  ESP8266 Core          - https://github.com/esp8266/Arduino
  ESP8266 I2C lib fixed - https://github.com/enjoyneering/ESP8266-I2C-Driver
  STM32 Core            - https://github.com/rogerclarkmelbourne/Arduino_STM32

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <MAX31855.h>

int32_t rawData_01  = 0;
int32_t rawData_02  = 0;

/*
- for harware spi

  MAX31855(cs)

  cs  - chip select, if CS low serial interface is enabled
*/
MAX31855 myMAX31855_01(3); //for ESP8266 use D3
MAX31855 myMAX31855_02(4); //for ESP8266 use D4

void setup()
{
  Serial.begin(115200);

  /* start MAX31855 */
  myMAX31855_01.begin();
  myMAX31855_02.begin();

  while (myMAX31855_01.getChipID() != MAX31855_ID)
  {
    Serial.println(F("MAX6675_01 error")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("MAX6675_01 OK"));
}

void loop()
{
  while (myMAX31855_01.detectThermocouple() != MAX31855_THERMOCOUPLE_OK)
  {
    switch (myMAX31855_01.detectThermocouple())
    {
      case MAX31855_THERMOCOUPLE_SHORT_TO_VCC:
        Serial.println(F("Thermocouple_01 short to VCC"));
        break;

      case MAX31855_THERMOCOUPLE_SHORT_TO_GND:
        Serial.println(F("Thermocouple_01 short to GND"));
        break;

      case MAX31855_THERMOCOUPLE_NOT_CONNECTED:
        Serial.println(F("Thermocouple_01 not connected"));
        break;

      case MAX31855_THERMOCOUPLE_UNKNOWN:
        Serial.println(F("Thermocouple_01 unknown error, check spi cable"));
        break;
    }
    delay(5000);
  }

  rawData_01 = myMAX31855_01.readRawData();
  rawData_02 = myMAX31855_02.readRawData();

  Serial.print(F("Chip_ID_01: "));
  Serial.println(myMAX31855_01.getChipID(rawData_01));

  Serial.print(F("Chip_ID_02: "));
  Serial.println(myMAX31855_02.getChipID(rawData_02));

  Serial.print(F("Cold_Junction_01: "));
  Serial.println(myMAX31855_01.getColdJunctionTemperature(rawData_01));

  Serial.print(F("Cold_Junction_02: "));
  Serial.println(myMAX31855_02.getColdJunctionTemperature(rawData_02));

  Serial.print(F("Thermocouple_01: "));
  Serial.println(myMAX31855_01.getTemperature(rawData_01));

  Serial.print(F("Thermocouple_02: "));
  Serial.println(myMAX31855_02.getTemperature(rawData_02));

  delay(5000);
}
