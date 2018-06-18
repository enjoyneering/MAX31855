/***************************************************************************************************/
/*
   This is an Arduino library for 14-bit MAX31855 K-Thermocouple to Digital Converter
   with 12-bit Cold Junction Compensation. Can work with hardware & bitbang 5Mhz SPI & sampling
   rate ~9..10Hz.

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
     
   written by : enjoyneering79
   sourse code: https://github.com/enjoyneering/MAX31855

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

#ifndef MAX31855_h
#define MAX31855_h

#if defined(ARDUINO) && ARDUINO >= 100 //arduino core v1.0 or later
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>              //use for PROGMEM Arduino AVR
#elif defined(ESP8266)
#include <pgmspace.h>                  //use for PROGMEM Arduino ESP8266
#elif defined(_VARIANT_ARDUINO_STM32_)
#include <avr/pgmspace.h>              //use for PROGMEM Arduino STM32
#endif

#include <SPI.h>

#define MAX31855_CONVERSION_POWER_UP_TIME   200    //in milliseconds
#define MAX31855_CONVERSION_TIME            100    //in milliseconds, 9..10Hz sampling rate 
#define MAX31855_THERMOCOUPLE_RESOLUTION    0.25   //in deg.C per dac step
#define MAX31855_COLD_JUNCTION_RESOLUTION   0.0625 //in deg.C per dac step


#define MAX31855_ID                         31855
#define MAX31855_FORCE_READ_DATA            7      //force to read the data, 7 is unique because d2d1d0 can't be all high at the same time
#define MAX31855_ERROR                      2000   //returned value if any error happends


#define MAX31855_THERMOCOUPLE_OK            0
#define MAX31855_THERMOCOUPLE_SHORT_TO_VCC  1
#define MAX31855_THERMOCOUPLE_SHORT_TO_GND  2
#define MAX31855_THERMOCOUPLE_NOT_CONNECTED 3
#define MAX31855_THERMOCOUPLE_UNKNOWN       4


class MAX31855
{
  public:
   MAX31855(uint8_t cs, uint8_t so, uint8_t sck);                                    //sw spi
   MAX31855(uint8_t cs);                                                             //hw spi

   void     begin(void);
   uint8_t  detectThermocouple(int32_t rawValue = MAX31855_FORCE_READ_DATA);
   uint16_t getChipID(int32_t rawValue = MAX31855_FORCE_READ_DATA);
   float    getTemperature(int32_t rawValue = MAX31855_FORCE_READ_DATA);
   float    getColdJunctionTemperature(int32_t rawValue = MAX31855_FORCE_READ_DATA);
   int32_t  readRawData(void);
 
  private:
   bool    _useHardwareSPI;                                                          //true -> hw spi, false ->sw spi
   uint8_t _cs;
   uint8_t _so;
   uint8_t _sck;
};

#endif
