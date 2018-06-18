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

  Frameworks & Libraries:
  ATtiny Core           - https://github.com/SpenceKonde/ATTinyCore
  ESP8266 Core          - https://github.com/esp8266/Arduino
  ESP8266 I2C lib fixed - https://github.com/enjoyneering/ESP8266-I2C-Driver
  STM32 Core*           - https://github.com/rogerclarkmelbourne/Arduino_STM32

                          *STM32F103xxxx pins are NOT 5v tolerant, bi-directional
                           logic level converter is needed

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <MAX31855.h>

int32_t rawData = 0;

/*
- for soft spi

  MAX31855(cs, so, sck)

  cs  - chip select, if CS low serial interface is enabled
  so  - serial data output
  sck - serial clock input
*/
MAX31855 myMAX31855(3, 4, 7); //for ESP8266 use D3, D4, D7


void setup()
{
  Serial.begin(115200);

  /* start MAX31855 */
  myMAX31855.begin();

  while (myMAX31855.getChipID() != MAX31855_ID)
  {
    Serial.println(F("MAX6675 error")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("MAX6675 OK"));
}

void loop()
{
  while (myMAX31855.detectThermocouple() != MAX31855_THERMOCOUPLE_OK)
  {
    switch (myMAX31855.detectThermocouple())
    {
      case MAX31855_THERMOCOUPLE_SHORT_TO_VCC:
        Serial.println(F("Thermocouple short to VCC"));
        break;

      case MAX31855_THERMOCOUPLE_SHORT_TO_GND:
        Serial.println(F("Thermocouple short to GND"));
        break;

      case MAX31855_THERMOCOUPLE_NOT_CONNECTED:
        Serial.println(F("Thermocouple not connected"));
        break;

      case MAX31855_THERMOCOUPLE_UNKNOWN:
        Serial.println(F("Thermocouple unknown error, check spi cable"));
        break;
    }
    delay(5000);
  }

  rawData = myMAX31855.readRawData();

  Serial.print(F("Chip ID: "));
  Serial.println(myMAX31855.getChipID(rawData));

  Serial.print(F("Cold Junction: "));
  Serial.println(myMAX31855.getColdJunctionTemperature(rawData));

  Serial.print(F("Thermocouple: "));
  Serial.println(myMAX31855.getTemperature(rawData));

  delay(5000);
}
